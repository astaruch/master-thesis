# Remarks

Encountered problems:

- problematic fragment during parsing to Poco::URI()
  - https://a2plcpnl0399.prod.iad2.secureserver.net/~ra2bm64h38ry/banco.bradesco/index.php?id=Bradesco=prime=Exclusive=$#%#$%#%#%$#%$#%#$%#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%#%#%$#%$#%$#%#
  - https://a2plcpnl0618.prod.iad2.secureserver.net/~ycchchgf05h7/gerentevirtual/html/#%#$%#%#%$#%$#%#$%#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%$#%#%#%$#%$#%$#- https://account-verification.info/myaccount/websrc.php?locked=account_&amp;login_params=checked&amp;email_locked=&amp;step=@Id['3#p!D]wf4c]*0dwVch!P3n04T$&lt;%20@;48Qt0F`]%J{'f-q4GB%!UmENfs*
  - http://ec2-18-191-183-107.us-east-2.compute.amazonaws.com/8F7DSYGHFDSIF87DSTFGDSJFI8DS76FTSDYFUISFKJSDYF6S7FI/atualizacao/home.html#015734.shtml-%20tEc1rQkznrW2BBk%HJqt%%gf2-0CUeA4VlAc@HP91Lu#6uWc%NTw*!gch03V-HFt*#tS8AFPQxkop1xo!64XJ!rZnlgPSIwxCY35TuJ8XTmZtjRbN#sl%$!uwHI#BsEA0yoL!Qtp5c%h6H2qr-fUd$Fz3PXF@bkfIw6uKvwf8JWwVqATj99H9Oun*$KN6nThdv@rqQfsAWD*HPYrhuPmlNnI0AIsehx9E0B8w9K1LUM0NsSj@@6-4w7ybUTqXi!%2AQHYKCbyp*0kvrrCt5uU6uYh*eKJ4LrBVwCEaQap6$l
- *solution* is that we are not entering them into database
- probleatic query string => we are using only raw query and not trying to encode/decode it
- UTF-8 characters in db => set in db that we are accepting UTF8 - https://stackoverflow.com/questions/4867272/invalid-byte-sequence-for-encoding-utf8
