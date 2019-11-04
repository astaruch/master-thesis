#include "gtest/gtest.h"
#include "features/url_features.h"

TEST(HostIPAddress, NoIPAddress)
{
    url_features_t f("http://google.com");
    EXPECT_EQ(0., f.compute_value_ip_address());
}

TEST(HostIPAddress, ClassicDotForm)
{
    url_features_t f("http://138.197.136.185/bnc/National\%20Bank\%20Online.html");
    EXPECT_EQ(1., f.compute_value_ip_address());
}

TEST(HostIPAddress, OctalForm)
{
    url_features_t f("https://0254.0331.027.0356");
    EXPECT_EQ(1., f.compute_value_ip_address());
}

TEST(HostIPAddress, DecimalForm)
{
    url_features_t f("https://2899908590");
    EXPECT_EQ(1., f.compute_value_ip_address());
}

TEST(HostIPAddress, HexadecimalForm)
{
    url_features_t f("http://0x308f647/-credit-agricole-france/");
    EXPECT_EQ(1., f.compute_value_ip_address());
}

TEST(Length, ShortURL)
{
    url_features_t f("https://google.com");
    EXPECT_GT(0.1, f.compute_value_url_length());
}

TEST(Length, LongURL)
{
    url_features_t f("http://abielonline.com/wp-includes/alibaba/vqcr8bp0gud&amp;lc=1033&amp;id=64855&amp;mkt=en-us&amp;cbcxt=mai&amp;snsc.php?email=santmmafs@etttbt.com%20&amp;?email=rxeitsie@sbeilmep.com%20");
    EXPECT_EQ(1, f.compute_value_url_length());
}

TEST(Length, LongHost)
{
    url_features_t f("http://googlegooglegooglegoogle.com");
    EXPECT_LT(0.6, f.compute_value_host_length());
}

TEST(Length, LongPath)
{
    url_features_t f("http://abielonline.com/aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    EXPECT_LT(0.8, f.compute_value_path_length());
}

TEST(Length, LongQuery)
{
    url_features_t f("http://abielonline.com/?aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    EXPECT_LT(0.8, f.compute_value_query_length());
}

TEST(Length, LongFragment)
{
    url_features_t f("http://abielonline.com/?q#abcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcaabcabcabcabcabcabcaabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabc");
    EXPECT_LT(0.8, f.compute_value_fragment_length());
}

TEST(UserInfo, HasUserInfo)
{
    url_features_t f("http://postgres:password@www.pgadmin.com");
    EXPECT_EQ(1., f.compute_value_user_info());
}

TEST(UserInfo, PhishingUserInfo)
{
    url_features_t f("https://support.microsoft.com@2782399.azureedge.net");
    EXPECT_EQ(1., f.compute_value_user_info());
}

TEST(UserInfo, NoUserInfo)
{
    url_features_t f("http://google.com");
    EXPECT_EQ(0., f.compute_value_user_info());
}

TEST(UserInfo, RealURL)
{
    url_features_t f("ftp://cust-r2:Alpc2p3O@update.symantec.com/opt/content/onramp/minitri.flg");
    EXPECT_EQ(1., f.compute_value_user_info());
}

TEST(DomainCount, NormalURL)
{
    url_features_t f("http://google.com");
    EXPECT_EQ(0, f.compute_value_domain_count());
}

TEST(DomainCount, LotsOfSubdomain)
{
    url_features_t f("http://a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.com");
    EXPECT_LT(0.5, f.compute_value_domain_count());
}

TEST(DomainCount, InfinitySubdomains)
{
    url_features_t f("http://a.b.c.d.e.f.g.h.i.j.k.b.c.d.e.f.g.h.i.j.k.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.com");
    EXPECT_EQ(1, f.compute_value_domain_count());
}

TEST(HTTPSProtocol, Used)
{
    url_features_t f("https://google.com");
    EXPECT_EQ(0, f.compute_value_https_used());
}

TEST(HTTPSProtocol, Unused)
{
    url_features_t f("http://google.com");
    EXPECT_EQ(1, f.compute_value_https_used());
}


TEST(HTTPSProtocol, FTP)
{
    url_features_t f("ftp://google.com");
    EXPECT_EQ(1, f.compute_value_https_used());
}

TEST(HTTPSProtocol, HTTPSCaps)
{
    url_features_t f("HTTPS://google.com");
    EXPECT_EQ(0, f.compute_value_https_used());
}

TEST(ExtraHTTPS, WWW_HTTPS)
{
   url_features_t f("http://www.https-myetherwallet.net");
    EXPECT_EQ(1, f.compute_value_extra_https());
}
TEST(ExtraHTTPS, HTTPS)
{
   url_features_t f("http://https.mail.office.kyqjsd9dii74rmzs8g3su.46k2iqngqg00afsmaieyxyh.xwp14rfgoqn3tss8wc8wtp6i7.68lyyks730bhfn2hr4cbjxoqa9i.6uh8nzkrl7h2hhz1xcsvm.z723f6ydtns2gmx49pms667d1w.nxtedn7i9dhsa8694qsxiva2b.ddssdsd.duckdns.org");
    EXPECT_EQ(1, f.compute_value_extra_https());
}

TEST(ExtraHTTPS, NoToken)
{
   url_features_t f("http://google.com");
    EXPECT_EQ(0, f.compute_value_extra_https());
}

TEST(ShorteningService, bitlycom)
{
    url_features_t f("http://bitly.com/GH874$ag");
    EXPECT_EQ(1, f.compute_value_shortening_service());
}

TEST(ShorteningService, bitlygg)
{
    url_features_t f("http://bitly.gg/GH874$ag");
    EXPECT_EQ(1, f.compute_value_shortening_service());
}

TEST(ShorteningService, bitly)
{
    url_features_t f("http://bit.ly/GH874$ag");
    EXPECT_EQ(1, f.compute_value_shortening_service());
}


TEST(ShorteningService, BITly)
{
    url_features_t f("http://BIT.ly/GH874$ag");
    EXPECT_EQ(1, f.compute_value_shortening_service());
}

TEST(ShorteningService, google)
{
    url_features_t f("http://goo.gl/GH874$ag");
    EXPECT_EQ(1, f.compute_value_shortening_service());
}

TEST(ShorteningService, tinyurl)
{
    url_features_t f("http://tinyurl.com/GH874$ag");
    EXPECT_EQ(1, f.compute_value_shortening_service());
}

TEST(ShorteningService, tinycc)
{
    url_features_t f("http://tiny.cc/GH874$ag");
    EXPECT_EQ(1, f.compute_value_shortening_service());
}

TEST(ShorteningService, livechatinc)
{
    url_features_t f("http://livechatinc.com/GH874$ag");
    EXPECT_EQ(1, f.compute_value_shortening_service());
}

TEST(ShorteningService, isgd)
{
    url_features_t f("http://is.gd/GH874$ag");
    EXPECT_EQ(1, f.compute_value_shortening_service());
}

TEST(ShorteningService, soogd)
{
    url_features_t f("http://soo.gd/GH874$ag");
    EXPECT_EQ(1, f.compute_value_shortening_service());
}

TEST(ShorteningService, clickme)
{
    url_features_t f("http://clicky.me/GH874$ag");
    EXPECT_EQ(1, f.compute_value_shortening_service());
}

TEST(ShorteningService, cuttly)
{
    url_features_t f("http://cutt.ly/GH874$ag");
    EXPECT_EQ(1, f.compute_value_shortening_service());
}

TEST(ShorteningService, normal)
{
    url_features_t f("http://google.com/GH874$ag");
    EXPECT_EQ(0, f.compute_value_shortening_service());
}

TEST(NonSTDPort, Implicit80)
{
    url_features_t f("http://google.com/GH874$ag");
    EXPECT_EQ(0, f.compute_value_non_std_port());
}

TEST(NonSTDPort, Explicit80)
{
    url_features_t f("http://google.com:80/GH874$ag");
    EXPECT_EQ(0, f.compute_value_non_std_port());
}

TEST(NonSTDPort, Implicit443)
{
    url_features_t f("https://google.com/GH874$ag");
    EXPECT_EQ(0, f.compute_value_non_std_port());
}

TEST(NonSTDPort, Explicit443)
{
    url_features_t f("https://google.com:443/GH874$ag");
    EXPECT_EQ(0, f.compute_value_non_std_port());
}

TEST(NonSTDPort, NonSTDPort)
{
    url_features_t f("http://google.com:81/GH874$ag");
    EXPECT_EQ(1, f.compute_value_non_std_port());
}

TEST(NonSTDPort, NonSTDPort2)
{
    url_features_t f("http://google.com:22/GH874$ag");
    EXPECT_EQ(1, f.compute_value_non_std_port());
}

TEST(SpecialChars, PathZero)
{
    url_features_t f("http://google.com/GH874ag");
    EXPECT_EQ(0, f.compute_value_spec_char_path());
}

TEST(SpecialChars, PathSmall)
{
    url_features_t f("http://google.com/GH87$$@#4ag");
    EXPECT_GT(0.5, f.compute_value_spec_char_path());
}

TEST(SpecialChars, PathBig)
{
    url_features_t f("http://google.com/GH!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!874ag");
    EXPECT_EQ(1, f.compute_value_spec_char_path());
}

TEST(SpecialChars, QueryZero)
{
    url_features_t f("http://google.com/GH874ag?asgoihao34");
    EXPECT_EQ(0, f.compute_value_spec_char_query());
}

TEST(SpecialChars, QuerySmall)
{
    url_features_t f("http://google.com/GH874ag?asgoihao34");
    EXPECT_EQ(0, f.compute_value_spec_char_query());
}

TEST(SpecialChars, QueryBig)
{
    url_features_t f("http://google.com/GH874ag?asgoihao34!@%20%20^*^*&");
    EXPECT_LT(0.5, f.compute_value_spec_char_query());
}

TEST(SpecialChars, FragmentZero)
{
    url_features_t f("http://google.com/GH874ag?asgoihao34#agagag");
    EXPECT_EQ(0, f.compute_value_spec_char_fragment());
}

TEST(SpecialChars, FragmentSmall)
{
    url_features_t f("http://google.com/GH874ag?asgoihao34#ajg*gagagaga(");
    EXPECT_GT(0.5, f.compute_value_spec_char_fragment());
}

TEST(SpecialChars, FragmentBig)
{
    url_features_t f("http://google.com/GH874ag?asgoih#gjakgj#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$");
    EXPECT_EQ(1, f.compute_value_spec_char_fragment());
}

TEST(SpecialChars, HostZero)
{
    url_features_t f("http://google.com/");
    EXPECT_EQ(0, f.compute_value_spec_char_host());
}

TEST(SpecialChars, HostSmall)
{
    url_features_t f("http://goo-gle.com/");
    EXPECT_GT(0.6, f.compute_value_spec_char_host());
}

TEST(SpecialChars, HostBig)
{
    url_features_t f("http://paypal-yandex-seznam-google.com");
    EXPECT_EQ(1, f.compute_value_spec_char_host());
}

TEST(gTLD, com)
{
    url_features_t f("http://google.com");
    EXPECT_EQ(0, f.compute_value_gtld());
}

TEST(gTLD, cz)
{
    url_features_t f("http://google.cz");
    EXPECT_EQ(0, f.compute_value_gtld());
}

TEST(gTLD, sk)
{
    url_features_t f("http://google.sk");
    EXPECT_EQ(0, f.compute_value_gtld());
}

TEST(gTLD, hu)
{
    url_features_t f("http://google.hu");
    EXPECT_EQ(1, f.compute_value_gtld());
}

TEST(WWWPrefix, NoPrefix)
{
    url_features_t f("http://google.hu");
    EXPECT_EQ(0, f.compute_value_www_prefix());
}

TEST(WWWPrefix, Prefix)
{
    url_features_t f("http://www-google.hu");
    EXPECT_EQ(1, f.compute_value_www_prefix());
}

TEST(FourNumbers, HasFourNumbers)
{
    url_features_t f("http://1511300s.hol.es/1000655841.html");
    EXPECT_EQ(1, f.compute_value_four_numbers());
}

TEST(FourNumbers, DoesntHave)
{
    url_features_t f("http://seznam.hol.es/1000655841.html");
    EXPECT_EQ(0, f.compute_value_four_numbers());
}

TEST(SpecialKeywords, ZeroKeywords)
{
    url_features_t f("http://www.jingau.com.tw/it/webstaticmktg2014designfontPP/SansPayPalSansBig-Bold.woff/d6c14aa74fbcfee1d1f8ed7eab33f6ca/");
    EXPECT_EQ(0, f.compute_value_spec_keywords());
}

TEST(SpecialKeywords, OneKeyword)
{
    url_features_t f("http://roadtracksway.com/updatemailserversecurity/login.php");
    EXPECT_EQ(0.1, f.compute_value_spec_keywords());
}

TEST(SpecialKeywords, ManyKeywords)
{
    url_features_t f("http://www.quartier.com.ar/easyflexa/wallate/update_accounting/assetsfontsproductsaccountmyaccountwebsc_loginaccountmyaccountwebsc_logincountry/account/myaccount/websc_login/?country.x=");
    EXPECT_EQ(1, f.compute_value_spec_keywords());
}

TEST(Punycode, StartswithPunycode)
{
    url_features_t f("http://xn--mytherwalliet-4o2g.com");
    EXPECT_EQ(1, f.compute_value_punycode());
}

TEST(Punycode, ContainsPunycode)
{
    url_features_t f("http://appleid.apple.com.setup.ca-icloud.xn--q9jyb4c");
    EXPECT_EQ(1, f.compute_value_punycode());
}

TEST(Punycode, NoPunycode)
{
    url_features_t f("http://google.com");
    EXPECT_EQ(0, f.compute_value_punycode());
}

TEST(AllTest, ReturnVector)
{
    auto url = "http://google.com";
    Poco::URI parsed(url);
    uint64_t flags = ~0;
    bool ok = true;
    url_features_t f(url, parsed, flags, ok);

    auto x = f.compute_values_vec();
    std::vector<double> y{0.0,0.0204081633,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

    ASSERT_EQ(x.size(), y.size());
    for (int i = 0; i < x.size(); ++i) {
        EXPECT_FLOAT_EQ(x[i], y[i]) << "Vectors x and y differ at index " << i;
    }
}
