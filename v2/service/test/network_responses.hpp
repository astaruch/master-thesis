#pragma once

#ifndef PHISHSVC_NETWORK_RESPONSES_H
#define PHISHSVC_NETWORK_RESPONSES_H

#include <string>

// curl -s -I -X GET http://google.com
auto curl_http_google = R"(HTTP/1.1 301 Moved Permanently
Location: http://www.google.com/
Content-Type: text/html; charset=UTF-8
Date: Tue, 12 Nov 2019 18:19:30 GMT
Expires: Thu, 12 Dec 2019 18:19:30 GMT
Cache-Control: public, max-age=2592000
Server: gws
Content-Length: 219
X-XSS-Protection: 0
X-Frame-Options: SAMEORIGIN)";

// curl -s -I -X GET https://www.google.com
auto curl_https_google = R"(HTTP/2 200
date: Tue, 12 Nov 2019 19:39:26 GMT
expires: -1
cache-control: private, max-age=0
content-type: text/html; charset=ISO-8859-1
p3p: CP="This is not a P3P policy! See g.co/p3phelp for more info."
server: gws
x-xss-protection: 0
x-frame-options: SAMEORIGIN
set-cookie: 1P_JAR=2019-11-12-19; expires=Thu, 12-Dec-2019 19:39:26 GMT; path=/; domain=.google.com
set-cookie: NID=191=ZRW8ExUeh9_G3TzUbOQuIcxIBKWDSmWPK4isNmM_kMhIk-_dVQZqEsSmEG_ur0vkl8D_U0CIZtsAQW0uqPDKgbyPkjoNo-5vnPcprj_Exo6xqUmH549oGvL0sBDw7nZi53U1DxlrgI0tNZqsuof-jBRXqc6Fk0TalwMb-nNwx-I; expires=Wed, 13-May-2020 19:39:26 GMT; path=/; domain=.google.com; HttpOnly
alt-svc: quic=":443"; ma=2592000; v="46,43",h3-Q050=":443"; ma=2592000,h3-Q049=":443"; ma=2592000,h3-Q048=":443"; ma=2592000,h3-Q046=":443"; ma=2592000,h3-Q043=":443"; ma=2592000
accept-ranges: none
vary: Accept-Encoding)";

// curl -s -I -X GET https://content-security-policy.com
auto curl_csp = R"(HTTP/1.1 200 OK
Server: nginx
Date: Tue, 12 Nov 2019 18:53:04 GMT
Content-Type: text/html
Content-Length: 25446
Last-Modified: Thu, 31 Oct 2019 18:17:02 GMT
Connection: keep-alive
ETag: "5dbb251e-6366"
Strict-Transport-Security: max-age=31536000;
Content-Security-Policy: default-src 'none'; script-src 'self' ssl.google-analytics.com 'sha256-xzi4zkCjuC8lZcD2UmnqDG0vurmq12W/XKM5Vd0+MlQ='; style-src 'self' maxcdn.bootstrapcdn.com fonts.googleapis.com; font-src fonts.gstatic.com maxcdn.bootstrapcdn.com; img-src 'self' ssl.google-analytics.com;
Accept-Ranges: bytes)";

// curl -s -I -X GET https://github.com
auto curl_github_com = R"(HTTP/1.1 200 OK
Date: Tue, 12 Nov 2019 18:54:00 GMT
Content-Type: text/html; charset=utf-8
Transfer-Encoding: chunked
Server: GitHub.com
Status: 200 OK
Vary: X-PJAX
ETag: W/"cf6f51f730d9dc51df666f68925a1f37"
Cache-Control: max-age=0, private, must-revalidate
Set-Cookie: has_recent_activity=1; path=/; expires=Tue, 12 Nov 2019 19:54:00 -0000
Set-Cookie: _octo=GH1.1.1017330016.1573584840; domain=.github.com; path=/; expires=Fri, 12 Nov 2021 18:54:00 -0000
Set-Cookie: logged_in=no; domain=.github.com; path=/; expires=Sat, 12 Nov 2039 18:54:00 -0000; secure; HttpOnly
Set-Cookie: _gh_sess=UkhVdjVIbndPL2xNR0ozcDg0TmlsaWpMM2RJVkVsNEJsS21EWWloOFViUEJ5eWlFdGhTWS9BYWF3dVROWkhDaG1qRE90c3djZmxWdkhqUk1LQ3QwVm5OZUdZYW5HWHF1OE5qVGYzUjVudmhXVW94OVRVMmhxQmsvb0krTFFOdm50bnhoNkFkUWxmeFhOQVZKQ3ZGR3BBPT0tLWw3THBHTjZvREU3MFRiRDBHYnpTZVE9PQ%3D%3D--2a52455494174a43decaaab7880ab90b0f3761bf; path=/; secure; HttpOnly
X-Request-Id: d58e2327-c323-48c9-98c0-2e07114f940d
Strict-Transport-Security: max-age=31536000; includeSubdomains; preload
X-Frame-Options: deny
X-Content-Type-Options: nosniff
X-XSS-Protection: 1; mode=block
Referrer-Policy: origin-when-cross-origin, strict-origin-when-cross-origin
Expect-CT: max-age=2592000, report-uri="https://api.github.com/_private/browser/errors"
Content-Security-Policy: default-src 'none'; base-uri 'self'; block-all-mixed-content; connect-src 'self' uploads.github.com www.githubstatus.com collector.githubapp.com api.github.com www.google-analytics.com github-cloud.s3.amazonaws.com github-production-repository-file-5c1aeb.s3.amazonaws.com github-production-upload-manifest-file-7fdce7.s3.amazonaws.com github-production-user-asset-6210df.s3.amazonaws.com wss://live.github.com; font-src github.githubassets.com; form-action 'self' github.com gist.github.com; frame-ancestors 'none'; frame-src render.githubusercontent.com; img-src 'self' data: github.githubassets.com identicons.github.com collector.githubapp.com github-cloud.s3.amazonaws.com *.githubusercontent.com customer-stories-feed.github.com spotlights-feed.github.com; manifest-src 'self'; media-src 'none'; script-src github.githubassets.com; style-src 'unsafe-inline' github.githubassets.com
Vary: Accept-Encoding
X-GitHub-Request-Id: E790:BBA0:2D7164C:4459F38:5DCAFFC8)";

// curl -s -I -X GET https://paypal.com
auto curl_paypal_com = R"(HTTP/2 302
content-type: text/html
content-length: 161
location: https://www.paypal.com/
strict-transport-security: max-age=31536000; includeSubDomains)";

// curl -s -I -X GET https://developer.mozilla.org
auto curl_developer_mozzila_org = R"(HTTP/2 301
content-type: text/html; charset=utf-8
content-length: 0
cache-control: s-maxage=300, public, max-age=0
date: Tue, 12 Nov 2019 18:58:53 GMT
location: /en-US/
server: meinheld/0.6.1
strict-transport-security: max-age=63072000
x-content-type-options: nosniff
x-xss-protection: 1; mode=block
vary: Cookie
x-cache: Miss from cloudfront
via: 1.1 a83878ab138bfe1ba334c102434a7924.cloudfront.net (CloudFront)
x-amz-cf-pop: PRG50
x-amz-cf-id: YUK9njPoiCjsamd_ts5ypz8CcNvL7QJJv2074cpHamPQrjNaczDvLQ==)";

// dig +timeout=2 +dnssec +short google.com
auto dig_google = R"(216.58.201.110)";

// dig +timeout=2 +dnssec +short aenbauk.com
auto dig_something_else = "";

// dig +timeout=2 +dnssec +short nic.cz
auto dig_nic_cz = R"(217.31.205.50
A 13 2 1800 20191122114922 20191109064001 9334 nic.cz. USE6G59lnOp54y/fyRWfPXilOZy6FfD/0JTX0ue+guM4py1OUKIcfWfy oBYvf5WlLWXHYUZYvEXY5wQbzeKBzA==)";

// whois google.ro
auto whois_google_ro = R"([Querying whois.rotld.ro]
[whois.rotld.ro]

% The WHOIS service offered by ROTLD and the access to the records in the ROTLD WHOIS database
% are provided for information purposes and to be used within the scope of technical or administrative
% necessities of Internet operation or to remedy legal problems. The use for other purposes,
% in particular for advertising and domain hunting, is not permitted.

% Without prejudice to the above, it is explicitly forbidden to extract, copy and/or use or re-utilise
% in any form and by any means (electronically or not) the whole or a quantitatively or qualitatively
% substantial part of the contents of the WHOIS database without prior and explicit permission by ROTLD,
% nor in any attempt hereof, to apply automated, electronic processes to ROTLD (or its systems).

% ROTLD cannot, under any circumstances, be held liable in case the stored information would prove
% to be wrong, incomplete or not accurate in any sense.

% You agree that any reproduction and/or transmission of data for commercial purposes will always
% be considered as the extraction of a substantial part of the content of the WHOIS database.

% By submitting the query you agree to abide by this policy and accept that ROTLD can take measures
% to limit the use of its WHOIS services in order to protect the privacy of its registrants or the
% integrity of the database.

% The ROTLD WHOIS service on port 43 never discloses any information concerning the registrant.

% Registrant information can be obtained through use of the web-based whois service available from
% the ROTLD website www.rotld.ro


  Domain Name: google.ro
  Registered On: 2000-07-17
  Expires On: 2020-09-16
  Registrar: MarkMonitor Inc.
  Referral URL: www.markmonitor.com

  DNSSEC: Inactive

  Nameserver: ns1.google.com
  Nameserver: ns2.google.com
  Nameserver: ns3.google.com
  Nameserver: ns4.google.com

  Domain Status: UpdateProhibited)";

// whois staruch.sk
auto whois_staruch_sk = R"([Querying whois.sk-nic.sk]
[whois.sk-nic.sk]
Domain:                       staruch.sk
Registrant:                   WY-105405
Admin Contact:                WY-105405
Tech Contact:                 WEBG-0001
Registrar:                    WEBG-0001
Created:                      2018-09-26
Updated:                      2019-09-13
Valid Until:                  2020-09-26
Nameserver:                   ns1.wy.sk
Nameserver:                   ns2.wy.sk
Nameserver:                   ns1.wydns.net
Nameserver:                   ns2.wydns.net
EPP Status:                   ok

Registrar:                    WEBG-0001
Name:                         Webglobe - Yegon, s. r. o.
Organization:                 Webglobe - Yegon, s. r. o.
Organization ID:              36306444
Phone:                        +421.258101062
Email:                        sk-nic@webglobe.sk
Street:                       Stará Prievozská 2
City:                         Bratislava
Postal Code:                  821 09
Country Code:                 SK
Created:                      2017-09-01
Updated:                      2019-11-12

Contact:                      WY-105405
Registrar:                    WEBG-0001
Created:                      2018-09-26
Updated:                      2018-09-26)";

// whois google.com
auto whois_google_com = R"([Querying whois.verisign-grs.com]
[Redirected to whois.markmonitor.com]
[Querying whois.markmonitor.com]
[whois.markmonitor.com]
Domain Name: google.com
Registry Domain ID: 2138514_DOMAIN_COM-VRSN
Registrar WHOIS Server: whois.markmonitor.com
Registrar URL: http://www.markmonitor.com
Updated Date: 2019-09-09T08:39:04-0700
Creation Date: 1997-09-15T00:00:00-0700
Registrar Registration Expiration Date: 2028-09-13T00:00:00-0700
Registrar: MarkMonitor, Inc.
Registrar IANA ID: 292
Registrar Abuse Contact Email: abusecomplaints@markmonitor.com
Registrar Abuse Contact Phone: +1.2083895740
Domain Status: clientUpdateProhibited (https://www.icann.org/epp#clientUpdateProhibited)
Domain Status: clientTransferProhibited (https://www.icann.org/epp#clientTransferProhibited)
Domain Status: clientDeleteProhibited (https://www.icann.org/epp#clientDeleteProhibited)
Domain Status: serverUpdateProhibited (https://www.icann.org/epp#serverUpdateProhibited)
Domain Status: serverTransferProhibited (https://www.icann.org/epp#serverTransferProhibited)
Domain Status: serverDeleteProhibited (https://www.icann.org/epp#serverDeleteProhibited)
Registrant Organization: Google LLC
Registrant State/Province: CA
Registrant Country: US
Admin Organization: Google LLC
Admin State/Province: CA
Admin Country: US
Tech Organization: Google LLC
Tech State/Province: CA
Tech Country: US
Name Server: ns2.google.com
Name Server: ns3.google.com
Name Server: ns1.google.com
Name Server: ns4.google.com
DNSSEC: unsigned
URL of the ICANN WHOIS Data Problem Reporting System: http://wdprs.internic.net/
>>> Last update of WHOIS database: 2019-11-12T10:29:25-0800 <<<

For more information on WHOIS status codes, please visit:
  https://www.icann.org/resources/pages/epp-status-codes

If you wish to contact this domain’s Registrant, Administrative, or Technical
contact, and such email address is not visible above, you may do so via our web
form, pursuant to ICANN’s Temporary Specification. To verify that you are not a
robot, please enter your email address to receive a link to a page that
facilitates email communication with the relevant contact(s).

Web-based WHOIS:
  https://domains.markmonitor.com/whois

If you have a legitimate interest in viewing the non-public WHOIS details, send
your request and the reasons for your request to whoisrequest@markmonitor.com
and specify the domain name in the subject line. We will review that request and
may ask for supporting documentation and explanation.

The data in MarkMonitor’s WHOIS database is provided for information purposes,
and to assist persons in obtaining information about or related to a domain
name’s registration record. While MarkMonitor believes the data to be accurate,
the data is provided "as is" with no guarantee or warranties regarding its
accuracy.

By submitting a WHOIS query, you agree that you will use this data only for
lawful purposes and that, under no circumstances will you use this data to:
  (1) allow, enable, or otherwise support the transmission by email, telephone,
or facsimile of mass, unsolicited, commercial advertising, or spam; or
  (2) enable high volume, automated, or electronic processes that send queries,
data, or email to MarkMonitor (or its systems) or the domain name contacts (or
its systems).

MarkMonitor.com reserves the right to modify these terms at any time.

By submitting this query, you agree to abide by this policy.

MarkMonitor is the Global Leader in Online Brand Protection.

MarkMonitor Domain Management(TM)
MarkMonitor Brand Protection(TM)
MarkMonitor AntiCounterfeiting(TM)
MarkMonitor AntiPiracy(TM)
MarkMonitor AntiFraud(TM)
Professional and Managed Services

Visit MarkMonitor at https://www.markmonitor.com
Contact us at +1.8007459229
In Europe, at +44.02032062220
--)";

// whois google.cn
auto whois_google_cn = R"([Querying whois.cnnic.cn]
[whois.cnnic.cn]
Domain Name: google.cn
ROID: 20030311s10001s00033735-cn
Domain Status: clientDeleteProhibited
Domain Status: serverDeleteProhibited
Domain Status: serverUpdateProhibited
Domain Status: clientTransferProhibited
Domain Status: serverTransferProhibited
Registrant ID: ename_el7lxxxazw
Registrant: 北京谷翔信息技术有限公司
Registrant Contact Email: dns-admin@google.com
Sponsoring Registrar: 厦门易名科技股份有限公司
Name Server: ns2.google.com
Name Server: ns1.google.com
Name Server: ns3.google.com
Name Server: ns4.google.com
Registration Time: 2003-03-17 12:20:05
Expiration Time: 2021-03-17 12:48:36
DNSSEC: unsigned)";

// whois youtube.jp
auto whois_youtube_jp = R"([Querying whois.jprs.jp]
[whois.jprs.jp]
[ JPRS database provides information on network administration. Its use is    ]
[ restricted to network administration purposes. For further information,     ]
[ use 'whois -h whois.jprs.jp help'. To suppress Japanese output, add'/e'     ]
[ at the end of command, e.g. 'whois -h whois.jprs.jp xxx/e'.                 ]

Domain Information:
[Domain Name]                   YOUTUBE.JP

[Registrant]                    Google LLC

[Name Server]                   ns1.google.com
[Name Server]                   ns2.google.com
[Name Server]                   ns3.google.com
[Name Server]                   ns4.google.com
[Signing Key]

[Created on]                    2007/06/12
[Expires on]                    2020/06/30
[Status]                        Active
[Last Updated]                  2019/07/01 01:05:07 (JST)

Contact Information:
[Name]                          Google LLC
[Email]                         dns-admin@google.com
[Web Page]
[Postal code]                   94043
[Postal Address]                Mountain View
                                1600 Amphitheatre Parkway
                                CA
[Phone]                         16502530000
[Fax])";

// whois nic.cz
auto whois_nic_cz = R"([Querying whois.nic.cz]
[whois.nic.cz]
%  (c) 2006-2019 CZ.NIC, z.s.p.o.
%
% Intended use of supplied data and information
%
% Data contained in the domain name register, as well as information
% supplied through public information services of CZ.NIC association,
% are appointed only for purposes connected with Internet network
% administration and operation, or for the purpose of legal or other
% similar proceedings, in process as regards a matter connected
% particularly with holding and using a concrete domain name.
%
% Full text available at:
% http://www.nic.cz/page/306/intended-use-of-supplied-data-and-information/
%
% See also a search service at http://www.nic.cz/whois/
%
%
% Whoisd Server Version: 3.12.0
% Timestamp: Tue Nov 12 19:41:02 2019

domain:       nic.cz
registrant:   CZ-NIC
admin-c:      FEELA
admin-c:      MAPET
nsset:        CZ.NIC
keyset:       CZNIC
registrar:    REG-CZNIC
status:       Sponsoring registrar change forbidden
registered:   30.10.1997 01:00:00
changed:      22.11.2016 15:07:40
expire:       15.03.2027

contact:      CZ-NIC
org:          CZ.NIC, z.s.p.o.
name:         CZ.NIC, z.s.p.o.
address:      Milesovska 1136/5
address:      Praha 3
address:      130 00
address:      CZ
registrar:    REG-CZNIC
created:      17.10.2008 12:08:21
changed:      15.05.2018 21:32:00

contact:      FEELA
name:         Ondřej Filip
registrar:    REG-MOJEID
created:      04.01.2005 13:35:00
changed:      21.03.2019 16:29:17

contact:      MAPET
name:         Martin Peterka
registrar:    REG-MOJEID
created:      06.03.2002 18:11:00
changed:      26.04.2019 16:42:48

nsset:        CZ.NIC
nserver:      d.ns.nic.cz (193.29.206.1, 2001:678:1::1)
nserver:      a.ns.nic.cz (194.0.12.1, 2001:678:f::1)
nserver:      b.ns.nic.cz (194.0.13.1, 2001:678:10::1)
tech-c:       JTALIR
tech-c:       JAROMIR-TALIR
registrar:    REG-CZNIC
created:      09.06.2008 14:30:16
changed:      20.09.2013 11:18:20

contact:      JTALIR
name:         Jaromír Talíř
e-mail:       jaromir.talir@nic.cz
registrar:    REG-MOJEID
created:      26.10.2010 10:53:34
changed:      13.09.2018 15:46:35

contact:      JAROMIR-TALIR
name:         Jaromír Talíř
address:      Americká 23
address:      Praha 2
address:      120 00
address:      CZ
registrar:    REG-MOJEID
created:      01.12.2011 17:35:04
changed:      27.09.2017 17:02:28

keyset:       CZNIC
dnskey:       257 3 13 LM4zvjUgZi2XZKsYooDE0HFYGfWp242fKB+O8sLsuox8S6MJTowY8lBDjZD7JKbmaNot3+1H8zU9TrDzWmmHwQ==
tech-c:       JTALIR
registrar:    REG-CZNIC
created:      20.05.2016 16:13:26
)";

// whois google.com.br
auto whois_google_com_br = R"([Querying whois.registro.br]
[whois.registro.br]

% Copyright (c) Nic.br
%  The use of the data below is only permitted as described in
%  full by the terms of use at https://registro.br/termo/en.html ,
%  being prohibited its distribution, commercialization or
%  reproduction, in particular, to use it for advertising or
%  any similar purpose.
%  2019-11-12T15:42:10-03:00

domain:      google.com.br
owner:       Google Brasil Internet Ltda
ownerid:     06.990.590/0001-23
responsible: Domain Administrator
country:     BR
owner-c:     DOADM17
admin-c:     DOADM17
tech-c:      DOADM17
billing-c:   NAB51
nserver:     ns1.google.com
nsstat:      20191111 AA
nslastaa:    20191111
nserver:     ns2.google.com
nsstat:      20191111 AA
nslastaa:    20191111
nserver:     ns3.google.com
nsstat:      20191111 AA
nslastaa:    20191111
nserver:     ns4.google.com
nsstat:      20191111 AA
nslastaa:    20191111
created:     19990518 #162310
changed:     20190417
expires:     20200518
status:      published

nic-hdl-br:  DOADM17
person:      Domain Admin
e-mail:      ccops@markmonitor.com
country:     BR
created:     20100520
changed:     20180324

nic-hdl-br:  NAB51
person:      NameAction do Brasil
e-mail:      mail@nameaction.com
country:     BR
created:     20020619
changed:     20181011

% Security and mail abuse issues should also be addressed to
% cert.br, http://www.cert.br/ , respectivelly to cert@cert.br
% and mail-abuse@cert.br
%
% whois.registro.br accepts only direct match queries. Types
% of queries are: domain (.br), registrant (tax ID), ticket,
% provider, contact handle (ID), CIDR block, IP and ASN.)";

// whois i-dont-exist.com.cz
auto whois_doesnt_exists = R"([Querying whois.nic.cz]
[whois.nic.cz]
%  (c) 2006-2019 CZ.NIC, z.s.p.o.
%
% Intended use of supplied data and information
%
% Data contained in the domain name register, as well as information
% supplied through public information services of CZ.NIC association,
% are appointed only for purposes connected with Internet network
% administration and operation, or for the purpose of legal or other
% similar proceedings, in process as regards a matter connected
% particularly with holding and using a concrete domain name.
%
% Full text available at:
% http://www.nic.cz/page/306/intended-use-of-supplied-data-and-information/
%
% See also a search service at http://www.nic.cz/whois/
%
%
% Whoisd Server Version: 3.12.0

%ERROR:101: no entries found
%
% No entries found.
)";

// whois nic.sk
auto whois_nic_sk = R"([Querying whois.sk-nic.sk]
[whois.sk-nic.sk]
Domain:                       nic.sk
Registrant:                   H519529
Admin Contact:                H519529
Tech Contact:                 H519529
Registrar:                    WEBG-0001
Created:                      2003-09-30
Updated:                      2019-10-14
Valid Until:                  2021-09-30
Nameserver:                   ns1.nic.sk
Nameserver:                   ns2.nic.sk
Nameserver:                   ns3.nic.sk
Nameserver:                   ns4.nic.sk
EPP Status:                   clientTransferProhibited, clientUpdateProhibited, clientDeleteProhibited

Registrar:                    WEBG-0001
Name:                         Webglobe - Yegon, s. r. o.
Organization:                 Webglobe - Yegon, s. r. o.
Organization ID:              36306444
Phone:                        +421.258101062
Email:                        sk-nic@webglobe.sk
Street:                       Stará Prievozská 2
City:                         Bratislava
Postal Code:                  821 09
Country Code:                 SK
Created:                      2017-09-01
Updated:                      2019-11-12

Contact:                      H519529
Name:                         Vaclav Faltus
Organization:                 RODINKA SHOP, s. r. o.
Organization ID:              46467769
Phone:                        +421.948773000
Email:                        vaclav@faltus.sk
Street:                       Dlha 9
City:                         Ivanka pri Dunaji
Postal Code:                  90028
Country Code:                 SK
Registrar:                    WEBG-0001
Created:                      2018-02-03
Updated:                      2018-02-03)";

// whois --verbose kjmtelecom.com.br
auto whois_kjmtelecom_com_br = R"([Querying whois.registro.br]
[whois.registro.br]

% Copyright (c) Nic.br
%  The use of the data below is only permitted as described in
%  full by the terms of use at https://registro.br/termo/en.html ,
%  being prohibited its distribution, commercialization or
%  reproduction, in particular, to use it for advertising or
%  any similar purpose.
%  2019-11-15T16:00:30-03:00

domain:      kjmtelecom.com.br
owner:       Rosa Karina Souza Santana
ownerid:     003.211.135-51
country:     BR
owner-c:     RKSSA11
admin-c:     RKSSA11
tech-c:      RKSSA11
billing-c:   RKSSA11
nserver:     ns54.prodns.com.br
nsstat:      20191110 AA
nslastaa:    20191110
nserver:     ns55.prodns.com.br
nsstat:      20191110 AA
nslastaa:    20191110
saci:        yes
created:     20150820 #14574811
changed:     20190507
expires:     20220820
status:      published

nic-hdl-br:  RKSSA11
person:      Rosa Karina Souza Santana
e-mail:      adm.kjmtelecom@gmail.com
country:     BR
created:     20190405
changed:     20190405)";

// openssl s_client -connect www.google.com:443 2>/dev/null | openssl x509 -noout -subject -dates 2>/dev/null
auto ssl_www_google_com = R"(subject=C = US, ST = California, L = Mountain View, O = Google LLC, CN = www.google.com
notBefore=Oct 16 12:28:32 2019 GMT
notAfter=Jan  8 12:28:32 2020 GMT)";

// openssl s_client -connect google.com:443 2>/dev/null | openssl x509 -noout -subject -dates 2>/dev/null
auto ssl_google_com = R"(subject=C = US, ST = California, L = Mountain View, O = Google LLC, CN = *.google.com
notBefore=Oct 16 12:36:57 2019 GMT
notAfter=Jan  8 12:36:57 2020 GMT)";

// openssl s_client -connect google.com:80 2>/dev/null | openssl x509 -noout -subject -dates 2>/dev/null
auto ssl_empty = "";

// curl --max-time 2 -s "http://suggestqueries.google.com/complete/search?output=firefox&q=paypel" | jq . | sed -n 4p | egrep -o '[[:alnum:]]*'
auto similar_domain_paypel = "paypal";

// curl --max-time 2 -s "http://suggestqueries.google.com/complete/search?output=firefox&q=paypal" | jq . | sed -n 4p | egrep -o '[[:alnum:]]*'
auto similar_domain_paypal = "paypal";

// curl --max-time 2 -s "http://suggestqueries.google.com/complete/search?output=firefox&q=asgasgasgaasigjaigjaoisgagas" | jq . | sed -n 4p | egrep -o '[[:alnum:]]*'
auto similar_domain_empty = "";

// curl --max-time 2 -s "http://suggestqueries.google.com/complete/search?output=firefox&q=gaagle" | jq . | sed -n 4p | egrep -o '[[:alnum:]]*'
auto similar_domain_gaagle = "google";

// echo some.weird.domain.paypel.com | faup -f domain_without_tld
auto sld_som_weird_domain_paypel_com = "paypel";

// echo paypal.com | faup -f domain_without_tld
auto sld_paypal_com = "paypal";

// echo aiohsgoiashgoiashgoiahgoihaigo.com | faup -f domain_without_tld
auto sld_aiohsgoiashgoiashgoiahgoihaigo = "aiohsgoiashgoiashgoiahgoihaigo";

// echo gaagle.com | faup -f domain_without_tld
auto sld_gaagle = "gaagle";

// whois --verbose 216.58.201.110 | grep -i origin
//OriginAS:       AS15169
auto asn_google_com = "AS15169";

#endif // PHISHSVC_NETWORK_RESPONSES_H
