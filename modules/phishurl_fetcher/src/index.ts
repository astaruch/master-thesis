import 'reflect-metadata'
import * as fs from 'fs'
import { promisify } from 'util'
import * as path from 'path'
import { userInfo } from 'os'
import { createConnection } from 'typeorm'
import pino, { Logger } from 'pino'
import parse from 'csv-parse/lib/sync'
import { Phishtank } from './entity/Phishtank'

// eslint-disable-next-line camelcase
interface PhishtankRecordType {
  // eslint-disable-next-line camelcase
  phish_id: string
  url: string
  // eslint-disable-next-line camelcase
  phish_detail_url: string
  // eslint-disable-next-line camelcase
  submission_time: string
  verified: string
  // eslint-disable-next-line camelcase
  verification_time: string
  online: string
  target: string
}

const logger: Logger = pino({})

const readdirAsync = promisify(fs.readdir)

const getFilenames = async (inputDir: string): Promise<string[]> => {
  try {
    return await readdirAsync(inputDir)
  } catch (err) {
    logger.error(`Entered input directory doesn't exists: ${inputDir}`)
    return []
  }
}


const run = (): void => {
  const dir = path.join(__dirname, '../../../../master-thesis-phishtank-data/data/csv')
  getFilenames(dir).then(files => {
    files.forEach(file => {
      // process 1 csv file and insert into database
      const fullpath = path.join(dir, file)
      logger.info(fullpath)
      const csvContent = fs.readFileSync(fullpath)
      const records: PhishtankRecordType[] = parse(csvContent.toString('utf8'), {
        columns: true,
        delimiter: ',',
      })

      console.log(records)
    })
  })
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
