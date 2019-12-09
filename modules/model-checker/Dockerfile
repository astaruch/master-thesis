FROM ubuntu:bionic-20191029 as builder

RUN apt-get update && apt-get install -y python3 python3-pip
# RUN python -m pip install --upgrade pip

COPY requirements.txt /app/
COPY model_checker.py /app/
COPY pretrained_model.pkl /app/

WORKDIR /app

RUN pip3 install -r requirements.txt
RUN pyinstaller --clean --onefile --add-data pretrained_model.pkl:. --distpath bin --name model-checker model_checker.py

CMD ["ls -la"]

#################################
FROM ubuntu:bionic-20191029 as server

ARG PORT
ENV PORT ${PORT}

COPY --from=builder /app/bin/model-checker /app/model-checker

CMD ["sh", "-c", "/app/model-checker --server ${PORT}"]
