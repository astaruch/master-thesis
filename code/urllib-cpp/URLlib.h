#ifndef URLLIB_CPP_URLLIB_H
#define URLLIB_CPP_URLLIB_H

#include <string>

// TODO: use this go parser - https://github.com/golang/go/blob/master/src/net/url/url.go

class URLlib {
  class URL {
   public:
    struct UserInfo {
        std::string m_username;
        std::string m_password;
        bool m_password_set;
    };

    enum class Encode {
      Path,
      PathSegment,
      Host,
      Zone,
      UserPassword,
      QueryComponent,
      Fragment
    };

    const std::string &
    Scheme () const
    { return m_scheme; }

    const std::string &
    Path () const
    { return m_path; }

    const std::string &
    Fragment () const
    { return m_fragment; }

    const size_t &
    Port () const
    { return m_port; }

    const bool
    ForceQuery () const
    { return m_force_query; }

    std::string
    Username (const UserInfo &ui)
    {
      return ui.m_username;
    }

    std::string
    Password (const UserInfo &ui)
    {
      return ui.m_password;
    }

    // Maybe rawurl is of the form scheme:path.
    // (Scheme must be [a-zA-Z][a-zA-Z0-9+-.]*)
    // If so, return scheme, path; else return "", rawurl.
    bool
    ParseScheme (const std::string &uri);

    std::tuple<std::string, std::string>
    Split (const std::string &src, const std::string &c, bool exclude_c = true) const;

    // Parse parses rawurl into a URL structure.
    //
    // The rawurl may be relative (a path, without a host) or absolute
    // (starting with a scheme). Trying to parse a hostname and path
    // without a scheme is invalid but may not necessarily return an
    // error, due to parsing ambiguities.
    std::tuple<URL, std::string>
    Parse (const std::string &raw_url);

    // ParseRequestURI parses rawurl into a URL structure. It assumes that
    // rawurl was received in an HTTP request, so the rawurl is interpreted
    // only as an absolute URI or an absolute path.
    // The string rawurl is assumed not to have a #fragment suffix.
    // (Web browsers strip #fragment before sending the URL to a web server.)
    std::tuple<URL, std::string>
    ParseRequestURI (const std::string &raw_url);

    // parse parses a URL from a string in one of two contexts. If
    // viaRequest is true, the URL is assumed to have arrived via an HTTP request,
    // in which case only absolute URLs or path-absolute relative URLs are allowed.
    // If viaRequest is false, all forms of relative URLs are allowed.
    std::tuple<URL, std::string>
    Parse (const std::string &raw_url, bool via_request);

    std::tuple<std::string, std::string, std::string>
    GetScheme (const std::string &raw_url);

    std::tuple<UserInfo, std::string, std::string>
    ParseAuthority (const std::string &authority);

    // parseHost parses host as an authority without user
    // information. That is, as host[:port].
    std::tuple<std::string, std::string>
    ParseHost (const std::string &host);

    // validOptionalPort reports whether port is either an empty string
    // or matches /^:\d*$/
    bool ValidOptionalPort (const std::string &port);

    // unescape unescapes a string; the mode specifies
    // which section of the URL string is being unescaped.
    std::tuple<std::string, std::string>
    Unescape (const std::string &str, Encode mode);

    std::string
    Escape (const std::string &str, Encode mode);

    // Return true if the specified character should be escaped when
    // appearing in a URL string, according to RFC 3986.
    //
    // Please be informed that for now shouldEscape does not check all
    // reserved characters correctly. See golang.org/issue/5684.
    bool
    ShouldEscape (char c, Encode mode);

    // validUserinfo reports whether s is a valid userinfo string per RFC 3986
    // Section 3.2.1:
    //     userinfo    = *( unreserved / pct-encoded / sub-delims / ":" )
    //     unreserved  = ALPHA / DIGIT / "-" / "." / "_" / "~"
    //     sub-delims  = "!" / "$" / "&" / "'" / "(" / ")"
    //                   / "*" / "+" / "," / ";" / "="
    //
    // It doesn't validate pct-encoded. The caller does that via func unescape.
    bool
    ValidUserInfo (std::string str);

    // User returns a Userinfo containing the provided username
    // and no password set.
    UserInfo
    UserNoPassword (const std::string &username);

    // UserPassword returns a Userinfo containing the provided username
    // and password.
    //
    // This functionality should only be used with legacy web sites.
    // RFC 2396 warns that interpreting Userinfo this way
    // ``is NOT RECOMMENDED, because the passing of authentication
    // information in clear text (such as URI) has proven to be a
    // security risk in almost every case where it has been used.''
    UserInfo
    UserPassword (const std::string &username, const std::string &password);

    bool
    IsHex (char c);
    int
    UnHex (char c);

    // setPath sets the Path and RawPath fields of the URL based on the provided
    // escaped path p. It maintains the invariant that RawPath is only specified
    // when it differs from the default encoding of the path.
    // For example:
    // - setPath("/foo/bar")   will set Path="/foo/bar" and RawPath=""
    // - setPath("/foo%2fbar") will set Path="/foo/bar" and RawPath="/foo%2fbar"
    // setPath will return an error only if the provided path contains an invalid
    // escaping.
    std::string
    SetPath (const std::string &path);

   private:

    std::string m_scheme;
    std::string m_opaque;
    UserInfo m_user;
    std::string m_host;
    std::string m_path;
    std::string m_raw_path;
    bool m_force_query;
    std::string m_raw_query;
    std::string m_fragment;
    size_t m_port;
  };

};

#endif //URLLIB_CPP_URLLIB_H
