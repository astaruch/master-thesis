import 'reflect-metadata'
import { createConnection } from 'typeorm'
import pino, { Logger } from 'pino'
import { Phishtank } from './entity/Phishtank'

const logger: Logger = pino({})

const run = (): void => {
  createConnection().then(async connection => {
    logger.info('Inserting a new record into the database...')

    const phishtankRecord = new Phishtank()
    phishtankRecord.phishId = 6014538
    phishtankRecord.url = 'https://outletdeprimaveira.com/index.php?produto=489466851'
    phishtankRecord.phishDetailUrl = 'http://www.phishtank.com/phish_detail.php?phish_id=6014538'
    phishtankRecord.submssionTime = '2019-04-17T20:36:17+00:00'
    phishtankRecord.verificationTime = '2019-04-17T20:37:05+00:00'
    phishtankRecord.online = true
    phishtankRecord.target = 'Other'


    await connection.manager.save(phishtankRecord)
    logger.info(`Saved a new user with id: ${phishtankRecord.id}`)

    logger.info('Loading users from the database...')
    const records = await connection.manager.find(Phishtank)
    logger.info('Loaded users: ', records)

    logger.info('Here you can setup and run express/koa/any other framework.')
  }).catch(error => logger.info(error))
}

if (require.main === module) {
  run()
}
