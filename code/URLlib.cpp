#include <algorithm>
#include <tuple>
#include <cstring>
#include "URLlib.h"

bool
URLlib::URL::ParseScheme(const std::string &uri)
{
    m_scheme = "";
    for (size_t i = 0; i < uri.length(); ++i)
    {
        const char &c = uri[i];
        if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
        {
            continue;
        }

        if (('0' <= c && c <= '9') || c == '+' || c == '-' || c == '.')
        {
            if (i == 0)
            {
                return true;
            }
        }

        if (c == ':')
        {
            if (i == 0)
            {
                // TODO: error, missing protocol scheme
                return false;
            }
            m_scheme = uri.substr(0, i);
            return true;
        }
        // we have encountered an invalid character,
        // so there is no valid scheme
        return true;
    }
    return true;
}

std::tuple<std::string, std::string>
URLlib::URL::Split(const std::string &src, const std::string &c, bool exclude_c) const
{
    auto pos = src.find(c);
    if (pos == std::string::npos)
    {
        return std::make_tuple(src, "");
    }
    else
    {
        return exclude_c ?
               std::make_tuple(src.substr(0, pos), src.substr(pos + c.length())) :
               std::make_tuple(src.substr(0, pos), src.substr(pos));
    }
}

std::tuple<URLlib::URL, std::string>
URLlib::URL::Parse(const std::string &raw_url)
{
    // Cut off #fragment
    auto[u, fragment] = Split(raw_url, "#", true);

    auto[url, error] = Parse(u, false);

    if (!error.empty())
    {
        // TODO: better error handling
        return {URLlib::URL(), "Error in Parse() #1"};
    }

    if (fragment.empty())
    {
        return {url, ""};
    }

    if (auto[fragment1, err1] = Unescape(fragment, Encode::Fragment); !err1.empty())
    {
        return {URLlib::URL(), "Error in Parse() #2"};
    }
    else
    {
        url.m_fragment = fragment1;
    }
    return {url, ""};
}

std::tuple<URLlib::URL, std::string>
URLlib::URL::ParseRequestURI(const std::string &raw_url)
{
    if (auto[url, err] = Parse(raw_url, true); !err.empty())
    {
        return {URLlib::URL(), err};
    }
    else
    {
        return {url, ""};
    }
}

std::tuple<URLlib::URL, std::string>
URLlib::URL::Parse(const std::string &raw_url, bool via_request)
{
    if (raw_url.empty() && via_request)
    {
        return {{},
                {"empty url", ""}};
    }

    URL url;
    if (raw_url == "*")
    {
        url.m_path = "*";
        return {url, {}};
    }

    auto[scheme, rest, err] = GetScheme(raw_url);
    if (!err.empty())
    {
        return {URL(), err};
    }
    std::transform(scheme.begin(), scheme.end(), url.m_scheme.begin(), ::tolower);

    if (rest.back() == '?' && std::count(rest.begin(), rest.end(), '?'))
    {
        url.m_force_query = true;
        rest.pop_back();
    }
    else
    {
        auto[rest2, raw_query] = Split(rest, "?", true);
        rest = std::move(rest2);
        url.m_raw_query = std::move(raw_query);
    }

    if (rest.front() == '/')
    {
        // We consider rootless paths per RFC 3986 as opaque.
        if (!url.m_scheme.empty())
        {
            url.m_opaque = std::move(rest);
            return {url, ""};
        }
        if (via_request)
        {
            return {URL(), "invalid URI for request"};
        }

        // Avoid confusion with malformed schemes, like cache_object:foo/bar.
        // See golang.org/issue/16822.
        //
        // RFC 3986, §3.3:
        // In addition, a URI reference (Section 4.1) may be a relative-path reference,
        // in which case the first path segment cannot contain a colon (":") character.
        size_t colon = rest.find(':');
        size_t slash = rest.find('/');
        if (colon >= 0 && (slash < 0 || colon < slash))
        {
            // First path segment has colon. Not allowed in relative URL.
            return {URL(), "first path segment in URL can't contain colon"};
        }
    }

    if ((!url.m_scheme.empty() || (!via_request && rest.find("///") != 0)) &&
        rest.find("//") == 0)
    {
        auto[authority, rest3] = Split(rest.substr(2), "/", false);
        auto[user, host, err1] = ParseAuthority(authority);
        if (!err1.empty())
        {
            return {URL(), err1};
        }
        url.m_user = std::move(user);
        url.m_host = std::move(host);

    }
    // Set Path and, optionally, RawPath.
    // RawPath is a hint of the encoding of Path. We don't want to set it if
    // the default escaping of Path is equivalent, to help make sure that people
    // don't rely on it in general.
    if (auto err1 = SetPath(rest); !err.empty())
    {
        return {URL(), err};
    }
    return {url, ""};
}

std::string
URLlib::URL::Escape(const std::string &str, URLlib::URL::Encode mode)
{
    size_t space_count = 0;
    size_t hex_count = 0;
    for (char c : str)
    {
        if (ShouldEscape(c, mode))
        {
            c == ' ' && mode == Encode::QueryComponent ? ++space_count : ++hex_count;
        }
    }

    if (space_count == 0 && hex_count == 0)
    {
        return str;
    }

    std::string escaped(str.length() + 2 * hex_count, 0);
    if (hex_count == 0)
    {
        escaped = str;
        std::replace(escaped.begin(), escaped.end(), ' ', '+');
        return escaped;
    }
    size_t j = 0;
    for (char c : str)
    {
        if (c == ' ' && mode == Encode::QueryComponent)
        {
            escaped[j++] = '+';
        }
        else if (ShouldEscape(c, mode))
        {
            escaped[j++] = '%';
            escaped[j++] = "0123456789ABCDEF"[c >> 4];
            escaped[j++] = "0123456789ABCDEF"[c & 15];
        }
        else
        {
            escaped[j++] = c;
        }
    }
    return escaped;
}

std::string
URLlib::URL::SetPath(const std::string &path)
{
    if (auto[parsed, err] = Unescape(path, Encode::Path); !err.empty())
    {
        return err;
    }
    else
    {
        m_path = parsed;
        if (auto escaped = Escape(parsed, Encode::Path); path == escaped)
        {
            m_raw_path = "";
        }
        else
        {
            m_raw_path = path;
        }
    }
    return {};
}

std::tuple<std::string, std::string, std::string>
URLlib::URL::GetScheme(const std::string &raw_url)
{
    for (size_t i = 0; i < raw_url.length(); ++i)
    {
        const char &c = raw_url[i];
        if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
        {
            continue;
        }

        if (('0' <= c && c <= '9') || c == '+' || c == '-' || c == '.')
        {
            if (i == 0)
            {
                return {"", raw_url, ""};
            }
        }

        if (c == ':')
        {
            if (i == 0)
            {
                return {"", "", "missing protocol scheme"};
            }
            m_scheme = raw_url.substr(0, i);
            return {raw_url.substr(0, i), raw_url.substr(i + 1), ""};
        }

        // we have encountered an invalid character,
        // so there is no valid scheme
        return {"", raw_url, ""};
    }
    return {"", raw_url, ""};
}

bool
URLlib::URL::ValidUserInfo(std::string str)
{
    return std::all_of(str.cbegin(), str.cend(), [](const char c)
    {
        return isalnum(c) || std::strchr("-._:~!$&\'()*+,;=%@", c) != nullptr;
    });
}

URLlib::URL::UserInfo
URLlib::URL::UserNoPassword(const std::string &username)
{
    return {username, "", false};
}

URLlib::URL::UserInfo
URLlib::URL::UserPassword(const std::string &username, const std::string &password)
{
    return {username, password, true};
}

std::tuple<URLlib::URL::UserInfo, std::string, std::string>
URLlib::URL::ParseAuthority(const std::string &authority)
{
    auto at = authority.find_last_of('@');
    std::string err;
    std::string host;
    if (at == std::string::npos)
    {
        auto[host1, err1] = ParseHost(authority);
        err = std::move(err1);
        host = std::move(host1);
    }
    else
    {
        auto[host2, err2] = ParseHost(authority.substr(at + 1));
        err = std::move(err2);
        host = std::move(host2);
    }
    if (!err.empty())
    {
        return {UserInfo(), "", err};
    }
    if (at < 0)
    {
        return {UserInfo(), host, ""};
    }
    std::string user_info_str = authority.substr(0, at);
    if (!ValidUserInfo(user_info_str))
    {
        return {UserInfo(), "", "net/url: invalid userinfo"};
    }
    UserInfo user;
    if (user_info_str.find(':') != std::string::npos)
    {
        auto[user_info1, err1] = Unescape(user_info_str, Encode::UserPassword);
        if (!err1.empty())
        {
            return {UserInfo(), "", err1};
        }
        user = UserNoPassword(user_info_str);
    }
    else
    {
        auto[username, password] = Split(user_info_str, ":", true);
        auto[username1, err1] = Unescape(username, Encode::UserPassword);
        if (!err1.empty())
        {
            return {{}, "", err1};
        }
        auto[password1, err2] = Unescape(password, Encode::UserPassword);
        if (!err2.empty())
        {
            return {{}, "", err2};
        }
        user = UserPassword(username, password);
    }
    return {user, host, ""};
}

bool
URLlib::URL::ValidOptionalPort(const std::string &port)
{
    if (port.empty())
    {
        return true;
    }
    if (port[0] != ':')
    {
        return false;
    }
    std::all_of(port.begin() + 1, port.end(), [](char c) { return c < '0' || c > '9'; });
    return true;
}

std::tuple<std::string, std::string>
URLlib::URL::ParseHost(const std::string &host)
{
    if (host.front() == '[')
    {
        // Parse an IP-Literal in RFC 3986 and RFC 6874.
        // E.g., "[fe80::1]", "[fe80::1%25en0]", "[fe80::1]:80".
        size_t end_square_bracket = host.rfind(']');
        if (end_square_bracket == std::string::npos)
        {
            return {"", "missing ']' in host"};
        }

        std::string colon_port = host.substr(end_square_bracket + 1);
        if (!ValidOptionalPort(colon_port))
        {
            return {"", "invalid port 1234 after host"}; //TODO: change to variable port
        }

        // RFC 6874 defines that %25 (%-encoded percent) introduces
        // the zone identifier, and the zone identifier can use basically
        // any %-encoding it likes. That's different from the host, which
        // can only %-encode non-ASCII bytes.
        // We do impose some restrictions on the zone, to avoid stupidity
        // like newlines.
        size_t zone = host.find("%25", 0, end_square_bracket);
        if (zone >= 0)
        {
            auto[host1, err1] = Unescape(host.substr(0, zone), Encode::Host);
            if (!err1.empty())
            {
                return {"", err1};
            }
            auto[host2, err2] = Unescape(host.substr(zone, end_square_bracket), Encode::Zone);
            if (!err2.empty())
            {
                return {"", err2};
            }
            auto[host3, err3] = Unescape(host.substr(end_square_bracket), Encode::Host);
            if (!err3.empty())
            {
                return {"", err3};
            }
            return {host1 + host2 + host3, ""};
        }
    }
    auto[host2, err] = Unescape(host, Encode::Host);
    if (!err.empty())
    {
        return {"", err};
    }
    return {host2, ""};
}

bool
URLlib::URL::IsHex(char c)
{
    if ('0' <= c && c <= '9')
    {
        return true;
    }
    if ('a' <= c && c <= 'f')
    {
        return true;
    }
    if ('A' <= c && c <= 'F')
    {
        return true;
    }
    return false;
}

int
URLlib::URL::UnHex(char c)
{
    if ('0' <= c && c <= '9')
    {
        return c - '0';
    }
    if ('a' <= c && c <= 'f')
    {
        return c - 'a' + 10;
    }
    if ('A' <= c && c <= 'F')
    {
        return c - 'A' + 10;
    }
    return 0;
}

bool
URLlib::URL::ShouldEscape(char c, URLlib::URL::Encode mode)
{
    // §2.3 Unreserved characters (alphanum)
    if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9'))
    {
        return false;
    }

    if (mode == Encode::Host || mode == Encode::Zone)
    {
        // §3.2.2 Host allows
        //	sub-delims = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
        // as part of reg-name.
        // We add : because we include :port as part of host.
        // We add [ ] because we include [ipv6]:port as part of host.
        // We add < > because they're the only characters left that
        // we could possibly allow, and Parse will reject them if we
        // escape them (because hosts can't use %-encoding for
        // ASCII bytes).
        if (c == '!' || c == '$' || c == '&' || c == '\'' || c == '(' || c == ')' || c == '*' || c == '+' ||
            c == ',' || c == ';' || c == '=' || c == ':' || c == '[' || c == ']' || c == '<' || c == '>' ||
            c == '"')
        {
            return false;
        }
    }

    if (c == '-' || c == '_' || c == '.' || c == '~')
    {
        return false; // §2.3 Unreserved characters (mark)
    }

    if (c == '$' || c == '&' || c == '+' || c == ',' || c == '/' ||
        c == ':' || c == ';' || c == '=' || c == '?' || c == '@')
    {
        switch (mode)
        {
            case Encode::Path: // §3.3
                // The RFC allows : @ & = + $ but saves / ; , for assigning
                // meaning to individual path segments. This package
                // only manipulates the path as a whole, so we allow those
                // last three as well. That leaves only ? to escape.
                return c == '?';
            case Encode::PathSegment: // §3.3
                // The RFC allows : @ & = + $ but saves / ; , for assigning
                // meaning to individual path segments.
                return c == '/' || c == ';' || c == ',' || c == '?';
            case Encode::UserPassword: // §3.2.1
                // The RFC allows ';', ':', '&', '=', '+', '$', and ',' in
                // userinfo, so we must escape only '@', '/', and '?'.
                // The parsing of userinfo treats ':' as special so we must escape
                // that too.
                return c == '@' || c == '/' || c == '?' || c == ':';
            case Encode::QueryComponent: // §3.4
                // The RFC reserves (so we must escape) everything.
                return true;
            case Encode::Fragment: // §4.1
                // The RFC text is silent but the grammar allows
                // everything, so escape nothing.
                return false;
            case Encode::Host:
            case Encode::Zone:break;
        }
    }

    if (mode == Encode::Fragment)
    {
        // RFC 3986 §2.2 allows not escaping sub-delims. A subset of sub-delims are
        // included in reserved from RFC 2396 §2.2. The remaining sub-delims do not
        // need to be escaped. To minimize potential breakage, we apply two restrictions:
        // (1) we always escape sub-delims outside of the fragment, and (2) we always
        // escape single quote to avoid breaking callers that had previously assumed that
        // single quotes would be escaped. See issue #19917.
        if (c == '!' || c == '(' || c == ')' || c == '*')
        {
            return false;
        }
    }

    // Everything else must be escaped.
    return true;
}

std::tuple<std::string, std::string>
URLlib::URL::Unescape(const std::string &str, URLlib::URL::Encode mode)
{
    // Count %, check that they're well-formed.
    size_t n = 0; // n is number of %
    bool has_plus = false;
    for (size_t i = 0; i < str.size(); ++i)
    {
        if (str[i] == '%')
        {
            n++;
            if (i + 2 >= str.size() || !IsHex(str[i + 1]) || !IsHex(str[i + 2]))
            {
                auto error = std::string("Escaping error: ") + str.substr(i, 3);
                return {"", error};
            }
            // Per https://tools.ietf.org/html/rfc3986#page-21
            // in the host component %-encoding can only be used
            // for non-ASCII bytes.
            // But https://tools.ietf.org/html/rfc6874#section-2
            // introduces %25 being allowed to escape a percent sign
            // in IPv6 scoped-address literals. Yay.
            if (mode == Encode::Host && UnHex(str[i + 1]) < 8 && str.substr(i, 3) != "%25")
            {
                auto error = std::string("Escaping error: ") + str.substr(i, 3);
                return {"", error};
            }
            if (mode == Encode::Zone)
            {
                // RFC 6874 says basically "anything goes" for zone identifiers
                // and that even non-ASCII can be redundantly escaped,
                // but it seems prudent to restrict %-escaped bytes here to those
                // that are valid host name bytes in their unescaped form.
                // That is, you can use escaping in the zone identifier but not
                // to introduce bytes you couldn't just write directly.
                // But Windows puts spaces here! Yay.
                auto v = UnHex(str[i + 1]) << 4 | UnHex(str[i + 2]);
                if (str.substr(i, 3) != "%25" && v != ' ' && ShouldEscape((char) (v), Encode::Host))
                {
                    auto error = std::string("Escaping error: ") + str.substr(i, 3);
                    return {"", error};
                }
            }
            i += 2; // skipping 3 chars in total ("%25".length == 3)

        }
        else if (str[i] == '+')
        {
            has_plus = mode == Encode::QueryComponent;
        }
        else
        {
            if ((mode == Encode::Host || mode == Encode::Zone) && (int) (str[i]) < 0x80 && ShouldEscape(str[i], mode))
            {
                std::string error = std::string("Invalid character in host name: ") + str.substr(i, i + 1);
                return {"", error};
            }
        }
    }
    if (n == 0 && !has_plus)
    {
        return {str, ""};
    }
    std::string t(str.length() - 2 * n, 0);
    size_t j = 0;
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] == '%')
        {
            t[j++] = static_cast<char>(UnHex(str[i + 1]) << 4 | UnHex(str[i + 2]));
            i += 2;
        }
        else if (str[i] == '+')
        {
            t[j++] = mode == Encode::QueryComponent ? ' ' : '+';
        }
        else
        {
            t[j++] = str[i];
        }
    }
    return {t, ""};
}

