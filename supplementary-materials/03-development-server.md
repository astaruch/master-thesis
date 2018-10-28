I am using stratus.fi.muni.cz

1. Download an Ubuntun 18.01.1 LTS Server edition
2. Upload it to Datastores/Image
3. Create a new instance from this uploaded image
4. Create a new volatile disk in storage menu for that VM and attach it
5. Install VM

Then there setup SSH keys:

    ssh-keygen -t rsa -b 4096 -C "staruch.andrej@gmail.com"

Copy public key into github.

Checkout the repo:

    git clone git@github.com:astaruch/master-thesis-phishtank-data.git

Start a CRON job

    crontab -e