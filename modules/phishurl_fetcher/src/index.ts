import 'reflect-metadata'
import * as fs from 'fs'
// import { promisify } from 'util'
import * as path from 'path'
import { createConnection } from 'typeorm'
import pino, { Logger } from 'pino'
import parse from 'csv-parse/lib/sync'
import { Phishtank } from './entity/Phishtank'
import { LastUpdated, TableName } from './entity/LastUpdated'

interface IPhishtankRecordType {
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

const run = (): void => {
  createConnection().then(connection => {
    const dir = path.join(__dirname, '../../../../master-thesis-phishtank-data/data/csv')
    // eslint-disable-next-line no-sync
    const files = fs.readdirSync(dir)

    const promiseArray = files.map(file => {
      const fullpath = path.join(dir, file)
      // eslint-disable-next-line no-sync
      if (!fs.statSync(fullpath).isFile()) {
        logger.info(`Skipping non-file: ${fullpath}`)
      }
      logger.info(`Parsing ${fullpath}`)
      return connection.manager.getRepository(LastUpdated)
        .findOneOrFail({ tableName: TableName.PHISHTANK })
        .then(lastUpdadedRow => {
          logger.info(`Before: ${lastUpdadedRow.lastUpdated}`)
          lastUpdadedRow.lastUpdated.setSeconds(lastUpdadedRow.lastUpdated.getSeconds() + 1)
          logger.info(`After: ${lastUpdadedRow.lastUpdated}`)
          return connection.manager.getRepository(LastUpdated).save(lastUpdadedRow).then(() => logger.info('Saved.'))
        })
    })

    Promise.all(promiseArray).then(() => logger.info('Done.'))
  })
}


// const promises = files.map(file => {
//   const fullpath = path.join(dir, file)
//   // eslint-disable-next-line no-sync
//   if (!fs.statSync(fullpath).isFile()) {
//     logger.trace(`Skipping non-file: ${fullpath}`)
//     // return new Promise(null)
//   }
//   logger.info(`Parsing ${fullpath}`)

//   connection.manager
//     .getRepository(LastUpdated)
//     .findOneOrFail({ tableName: TableName.PHISHTANK })
//     .then(lastUpdatedRow => {
//       logger.info(`Before: ${lastUpdatedRow.lastUpdated}`)
//       lastUpdatedRow.lastUpdated.setSeconds(lastUpdatedRow.lastUpdated.getSeconds() + 1)
//       logger.info(`After: ${lastUpdatedRow.lastUpdated}`)
//       connection.manager.getRepository(LastUpdated).save(lastUpdatedRow).then(async () => {
//         logger.info('saved')
//         const ids = await connection.manager
//           .getRepository(Phishtank)
//           .createQueryBuilder('phishtank')
//           .select('phishtank.phishId')
//           .getMany()
//         logger.info(ids[0])

//         // eslint-disable-next-line no-sync
//         const csvContent = fs.readFileSync(fullpath)
//         const newRecords: IPhishtankRecordType[] = parse(csvContent.toString('utf8'), {
//           columns: true,
//           delimiter: ',',
//         })
//       })
//     })
// })
// }
// })

// const phishtankRecord = new Phishtank()
// phishtankRecord.phishId = 6014538
// phishtankRecord.url = 'https://outletdeprimaveira.com/index.php?produto=489466851'
// phishtankRecord.phishDetailUrl = 'http://www.phishtank.com/phish_detail.php?phish_id=6014538'
// phishtankRecord.submssionTime = '2019-04-17T20:36:17+00:00'
// phishtankRecord.verificationTime = '2019-04-17T20:37:05+00:00'
// phishtankRecord.online = true
// phishtankRecord.target = 'Other'


// await connection.manager.save(phishtankRecord)
// logger.info(`Saved a new user with id: ${phishtankRecord.id}`)

// logger.info('Loading users from the database...')
// const records = await connection.manager.find(Phishtank)
// logger.info('Loaded users: ', records)

// logger.info('Here you can setup and run express/koa/any other framework.')
// }).catch(error => logger.info(error))
// }

if (require.main === module) {
  run()
}
