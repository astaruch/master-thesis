FROM mhart/alpine-node:12.13.1 as builder

COPY .eslintrc.js /app/
COPY package* /app/
COPY src /app/src
COPY test /app/test

WORKDIR /app

RUN npm install && \
    npm run test && \
    npm run lint

CMD ["node", "/app/src/index.js", "--help"]

#####################################
FROM mhart/alpine-node:slim-12.13.1 as server

ARG PORT
ENV PORT ${PORT}

COPY --from=builder /app /app

CMD ["sh", "-c", "node app/src/index.js --server ${PORT}"]
