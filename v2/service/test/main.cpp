#include <iostream>
#include "gtest/gtest.h"

#include "features/all_features.h"


TEST(HostIPAddress, NoIPAddress)
{
    feature::ip_address f;
    f.set_url("http://google.com");
    EXPECT_EQ(0., f.compute_value());
}

TEST(HostIPAddress, ClassicDotForm)
{
    feature::ip_address f;
    f.set_url("http://138.197.136.185/bnc/National\%20Bank\%20Online.html");
    EXPECT_EQ(1., f.compute_value());
}

TEST(HostIPAddress, OctalForm)
{
    feature::ip_address f;
    f.set_url("https://0254.0331.027.0356");
    EXPECT_EQ(1., f.compute_value());
}

TEST(HostIPAddress, DecimalForm)
{
    feature::ip_address f;
    f.set_url("https://2899908590");
    EXPECT_EQ(1., f.compute_value());
}

TEST(HostIPAddress, HexadecimalForm)
{
    feature::ip_address f;
    f.set_url("http://0x308f647/-credit-agricole-france/");
    EXPECT_EQ(1., f.compute_value());
}

TEST(Length, ShortURL)
{
    feature::url_length f;
    f.set_url("https://google.com");
    EXPECT_GT(0.1, f.compute_value());
}

TEST(Length, LongURL)
{
    feature::url_length f;
    f.set_url("http://abielonline.com/wp-includes/alibaba/vqcr8bp0gud&amp;lc=1033&amp;id=64855&amp;mkt=en-us&amp;cbcxt=mai&amp;snsc.php?email=santmmafs@etttbt.com%20&amp;?email=rxeitsie@sbeilmep.com%20");
    EXPECT_EQ(1, f.compute_value());
}

TEST(Length, ShortHost)
{
    feature::host_length f;
    f.set_url("http://google.com");
    EXPECT_GT(0.01, f.compute_value());
}


TEST(Length, NoPath)
{
    feature::path_length f;
    f.set_url("http://abielonline.com/");
    EXPECT_GT(0.01, f.compute_value());
}

TEST(Length, NoQuery)
{
    feature::query_length f;
    f.set_url("http://abielonline.com");
    EXPECT_GT(0.01, f.compute_value());
}

TEST(Length, NoFragment)
{
    feature::fragment_length f;
    f.set_url("http://abielonline.com/");
    EXPECT_GT(0.01, f.compute_value());
}

TEST(UserInfo, HasUserInfo)
{
    feature::user_info f;
    f.set_url("http://postgres:password@www.pgadmin.com");
    EXPECT_EQ(1, f.compute_value());
}

TEST(UserInfo, PhishingUserInfo)
{
    feature::user_info f;
    f.set_url("https://support.microsoft.com@2782399.azureedge.net");
    EXPECT_EQ(1, f.compute_value());
}

TEST(UserInfo, NoUserInfo)
{
    feature::user_info f;
    f.set_url("http://google.com");
    EXPECT_EQ(0, f.compute_value());
}

TEST(UserInfo, RealURL)
{
    feature::user_info f;
    f.set_url("ftp://cust-r2:Alpc2p3O@update.symantec.com/opt/content/onramp/minitri.flg");
    EXPECT_EQ(1, f.compute_value());
}

TEST(DomainCount, NormalURL)
{
    feature::domain_count f;
    f.set_url("http://google.com");
    EXPECT_EQ(0, f.compute_value());
}

TEST(DomainCount, LotsOfSubdomain)
{
    feature::domain_count f;
    f.set_url("http://a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.com");
    EXPECT_LT(0.5, f.compute_value());
}

TEST(DomainCount, InfinitySubdomains)
{
    feature::domain_count f;
    f.set_url("http://a.b.c.d.e.f.g.h.i.j.k.b.c.d.e.f.g.h.i.j.k.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.com");
    EXPECT_EQ(1, f.compute_value());
}

TEST(HTTPSProtocol, Used)
{
    feature::https_used f;
    f.set_url("https://google.com");
    EXPECT_EQ(0, f.compute_value());
}

TEST(HTTPSProtocol, Unused)
{
    feature::https_used f;
    f.set_url("http://google.com");
    EXPECT_EQ(1, f.compute_value());
}


TEST(HTTPSProtocol, FTP)
{
    feature::https_used f;
    f.set_url("ftp://google.com");
    EXPECT_EQ(1, f.compute_value());
}

TEST(HTTPSProtocol, HTTPSCaps)
{
    feature::https_used f;
    f.set_url("HTTPS://google.com");
    EXPECT_EQ(0, f.compute_value());
}

TEST(ExtraHTTPS, WWW_HTTPS)
{
    feature::extra_https f;
    f.set_url("http://www.https-myetherwallet.net");
    EXPECT_EQ(1, f.compute_value());
}
TEST(ExtraHTTPS, HTTPS)
{
    feature::extra_https f;
    f.set_url("http://https.mail.office.kyqjsd9dii74rmzs8g3su.46k2iqngqg00afsmaieyxyh.xwp14rfgoqn3tss8wc8wtp6i7.68lyyks730bhfn2hr4cbjxoqa9i.6uh8nzkrl7h2hhz1xcsvm.z723f6ydtns2gmx49pms667d1w.nxtedn7i9dhsa8694qsxiva2b.ddssdsd.duckdns.org");
    EXPECT_EQ(1, f.compute_value());
}

TEST(ExtraHTTPS, NoToken)
{
    feature::extra_https f;
    f.set_url("http://google.com");
    EXPECT_EQ(0, f.compute_value());
}

TEST(ShorteningService, bitlycom)
{
    feature::shortening_service f;
    f.set_url("http://bitly.com/GH874$ag");
    EXPECT_EQ(1, f.compute_value());
}

TEST(ShorteningService, bitlygg)
{
    feature::shortening_service f;
    f.set_url("http://bitly.gg/GH874$ag");
    EXPECT_EQ(1, f.compute_value());
}

TEST(ShorteningService, bitly)
{
    feature::shortening_service f;
    f.set_url("http://bit.ly/GH874$ag");
    EXPECT_EQ(1, f.compute_value());
}


TEST(ShorteningService, BITly)
{
    feature::shortening_service f;
    f.set_url("http://BIT.ly/GH874$ag");
    EXPECT_EQ(1, f.compute_value());
}

TEST(ShorteningService, google)
{
    feature::shortening_service f;
    f.set_url("http://goo.gl/GH874$ag");
    EXPECT_EQ(1, f.compute_value());
}

TEST(ShorteningService, tinyurl)
{
    feature::shortening_service f;
    f.set_url("http://tinyurl.com/GH874$ag");
    EXPECT_EQ(1, f.compute_value());
}

TEST(ShorteningService, tinycc)
{
    feature::shortening_service f;
    f.set_url("http://tiny.cc/GH874$ag");
    EXPECT_EQ(1, f.compute_value());
}

TEST(ShorteningService, livechatinc)
{
    feature::shortening_service f;
    f.set_url("http://livechatinc.com/GH874$ag");
    EXPECT_EQ(1, f.compute_value());
}

TEST(ShorteningService, isgd)
{
    feature::shortening_service f;
    f.set_url("http://is.gd/GH874$ag");
    EXPECT_EQ(1, f.compute_value());
}

TEST(ShorteningService, soogd)
{
    feature::shortening_service f;
    f.set_url("http://soo.gd/GH874$ag");
    EXPECT_EQ(1, f.compute_value());
}

TEST(ShorteningService, clickme)
{
    feature::shortening_service f;
    f.set_url("http://clicky.me/GH874$ag");
    EXPECT_EQ(1, f.compute_value());
}

TEST(ShorteningService, cuttly)
{
    feature::shortening_service f;
    f.set_url("http://cutt.ly/GH874$ag");
    EXPECT_EQ(1, f.compute_value());
}

TEST(ShorteningService, normal)
{
    feature::shortening_service f;
    f.set_url("http://google.com/GH874$ag");
    EXPECT_EQ(0, f.compute_value());
}

TEST(NonSTDPort, Implicit80)
{
    feature::non_std_port f;
    f.set_url("http://google.com/GH874$ag");
    EXPECT_EQ(0, f.compute_value());
}

TEST(NonSTDPort, Explicit80)
{
    feature::non_std_port f;
    f.set_url("http://google.com:80/GH874$ag");
    EXPECT_EQ(0, f.compute_value());
}

TEST(NonSTDPort, Implicit443)
{
    feature::non_std_port f;
    f.set_url("https://google.com/GH874$ag");
    EXPECT_EQ(0, f.compute_value());
}

TEST(NonSTDPort, Explicit443)
{
    feature::non_std_port f;
    f.set_url("https://google.com:443/GH874$ag");
    EXPECT_EQ(0, f.compute_value());
}

TEST(NonSTDPort, NonSTDPort)
{
    feature::non_std_port f;
    f.set_url("http://google.com:81/GH874$ag");
    EXPECT_EQ(1, f.compute_value());
}

TEST(NonSTDPort, NonSTDPort2)
{
    feature::non_std_port f;
    f.set_url("http://google.com:22/GH874$ag");
    EXPECT_EQ(1, f.compute_value());
}

TEST(SpecialChars, PathZero)
{
    feature::spec_char_path f;
    f.set_url("http://google.com/GH874ag");
    EXPECT_EQ(0, f.compute_value());
}

TEST(SpecialChars, PathSmall)
{
    feature::spec_char_path f;
    f.set_url("http://google.com/GH87$$@#4ag");
    EXPECT_GT(0.5, f.compute_value());
}

TEST(SpecialChars, PathBig)
{
    feature::spec_char_path f;
    f.set_url("http://google.com/GH!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!874ag");
    EXPECT_EQ(1, f.compute_value());
}

TEST(SpecialChars, QueryZero)
{
    feature::spec_char_query f;
    f.set_url("http://google.com/GH874ag?asgoihao34");
    EXPECT_EQ(0, f.compute_value());
}

TEST(SpecialChars, QuerySmall)
{
    feature::spec_char_query f;
    f.set_url("http://google.com/GH874ag?asgoihao34");
    EXPECT_EQ(0, f.compute_value());
}

TEST(SpecialChars, QueryBig)
{
    feature::spec_char_query f;
    f.set_url("http://google.com/GH874ag?asgoihao34!@%20%20^*^*&");
    EXPECT_LT(0.5, f.compute_value());
}

TEST(SpecialChars, FragmentZero)
{
    feature::spec_char_fragment f;
    f.set_url("http://google.com/GH874ag?asgoihao34#agagag");
    EXPECT_EQ(0, f.compute_value());
}

TEST(SpecialChars, FragmentSmall)
{
    feature::spec_char_fragment f;
    f.set_url("http://google.com/GH874ag?asgoihao34#ajg*gagagaga(");
    EXPECT_GT(0.5, f.compute_value());
}

TEST(SpecialChars, FragmentBig)
{
    feature::spec_char_fragment f;
    f.set_url("http://google.com/GH874ag?asgoih#gjakgj#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$#$");
    EXPECT_EQ(1, f.compute_value());
}

TEST(SpecialChars, HostZero)
{
    feature::spec_char_host f;
    f.set_url("http://google.com/");
    EXPECT_EQ(0, f.compute_value());
}

TEST(SpecialChars, HostSmall)
{
    feature::spec_char_host f;
    f.set_url("http://goo-gle.com/");
    EXPECT_GT(0.6, f.compute_value());
}

TEST(SpecialChars, HostBig)
{
    feature::spec_char_host f;
    f.set_url("http://paypal-yandex-seznam-google.com");
    EXPECT_EQ(1, f.compute_value());
}

TEST(gTLD, com)
{
    feature::gtld f;
    f.set_url("http://google.com");
    EXPECT_EQ(0, f.compute_value());
}

TEST(gTLD, cz)
{
    feature::gtld f;
    f.set_url("http://google.cz");
    EXPECT_EQ(0, f.compute_value());
}

TEST(gTLD, sk)
{
    feature::gtld f;
    f.set_url("http://google.sk");
    EXPECT_EQ(0, f.compute_value());
}

TEST(gTLD, hu)
{
    feature::gtld f;
    f.set_url("http://google.hu");
    EXPECT_EQ(1, f.compute_value());
}

TEST(WWWPrefix, NoPrefix)
{
    feature::www_prefix f;
    f.set_url("http://google.hu");
    EXPECT_EQ(0, f.compute_value());
}

TEST(WWWPrefix, Prefix)
{
    feature::www_prefix f;
    f.set_url("http://www-google.hu");
    EXPECT_EQ(1, f.compute_value());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
