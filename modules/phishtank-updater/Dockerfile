FROM mhart/alpine-node:12.13.1 as builder

COPY .eslintrc.js /app/
COPY package* /app/
COPY src /app/src

WORKDIR /app

RUN npm install && \
    npm run lint

CMD ["node", "/app/src/index.js", "--help"]

#####################################
FROM mhart/alpine-node:slim-12.13.1 as cronjob

COPY --from=builder /app /app
COPY cronjob /etc/cron.d/update-phishtank
RUN chmod 0644 /etc/cron.d/update-phishtank
RUN touch /var/log/cron.log

# Sets the CWD for the TYPEORM env variables has correct paths
WORKDIR /app
CMD node /app/src/index.js --update | ./node_modules/.bin/pino-pretty && crond && tail -f /var/log/cron.log
