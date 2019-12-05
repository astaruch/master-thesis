# Data science

Use jupyter notebook for a data science

    docker pull jupyter/minimal-notebook
    docker-compose up -d notebook

After that, set up a password inside jupyter notebook

    $ docker logs notebook
    ...
    [C 07:11:59.969 NotebookApp] 
    
    To access the notebook, open this file in a browser:
        file:///home/jovyan/.local/share/jupyter/runtime/nbserver-6-open.html
    Or copy and paste one of these URLs:
        http://fce37882f18c:8888/?token=d678366c6db8c8cc3348add31372554bf0ea1bbf25ab68bc
     or http://127.0.0.1:8888/?token=d678366c6db8c8cc3348add31372554bf0ea1bbf25ab68bc
   ...

Go on the IP address of Docker container with entered port in docker-compose

    $ docker inspect notebook | grep IPAddress
    "IPAddress": "172.17.0.3"
    
Visit link <http://172.17.0.3:10000> and in lower part of the site setup with token `d678366c6db8c8cc3348add31372554bf0ea1bbf25ab68bc` your new password, so after next visit of site, you don't need a token (which is generated on each startup of a container).


# Python checker


    $ sudo apt install python3-pip virtualenv
    $ pip3 install --user virtualenv==16.1 # https://github.com/pyinstaller/pyinstaller/issues/4064
    $ virtualenv --python=python3 venv
    $ source venv/bin/activate
    $ pip install -r requirements.txt
    $ make pack
    $ bin/model-checker --help

