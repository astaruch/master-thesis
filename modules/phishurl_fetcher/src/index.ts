/* eslint-disable no-await-in-loop */
/* eslint-disable no-sync */
/* eslint-disable camelcase */
import 'reflect-metadata'
import * as fs from 'fs'
import * as path from 'path'
import { createConnection } from 'typeorm'
import pino, { Logger } from 'pino'
import parse from 'csv-parse/lib/sync'
import { Phishtank } from './entity/Phishtank'
import { LastUpdated, TableName } from './entity/LastUpdated'

interface IPhishtankCsvRecordType {
  phish_id: string
  url: string
  phish_detail_url: string
  submission_time: string
  verified: string
  verification_time: string
  online: string
  target: string
}

const logger: Logger = pino({})

const run = (): void => {
  createConnection().then(async connection => {
    const dir = path.join(__dirname, '../../../../master-thesis-phishtank-data/data/csv')
    const files = fs.readdirSync(dir)
    for (let i = 0; i < files.length; i++) {
      const fullpath = path.join(dir, files[i])
      if (!fs.statSync(fullpath).isFile()) {
        logger.info(`Skipping non-file: ${fullpath}`)
        continue
      }
      logger.info(`Parsing ${fullpath}`)
      let lastDate
      try {
        const lastUpdatedRow = await connection
          .manager
          .getRepository(LastUpdated)
          .findOneOrFail({ tableName: TableName.PHISHTANK })
        lastDate = lastUpdatedRow.lastUpdated
      } catch (err) {
        logger.info('LastUpdate table doesn\'t exists. Using epoch time zero as lastUpdate.')
        lastDate = new Date(0)
        await connection
          .manager
          .getRepository(LastUpdated)
          .insert({ tableName: TableName.PHISHTANK, lastUpdated: lastDate.toISOString() })
      }

      // remove '.csv' extension and create same type as in database
      const currentDate = new Date(files[i].substr(0, files[i].length - 4))

      if (currentDate > lastDate) {
        logger.info(`Updating database with new csv: ${files[i]}`)

        const phishtankDb = await connection
          .manager
          .getRepository(Phishtank)
          .createQueryBuilder('phishtank')
          .getMany()

        const phishtankDbIds = phishtankDb
          .filter(dbRow => dbRow)
          .map(dbRow => dbRow.phishId)

        const phishtankDbOnlineIds = phishtankDb
          .filter(dbRow => dbRow && dbRow.online)
          .map(dbRow => dbRow.phishId)

        const csvContent = fs.readFileSync(fullpath)
        const csvRecords: IPhishtankCsvRecordType[] = parse(csvContent.toString('utf8'), {
          columns: true,
          delimiter: ',',
        })
        const phishtankCvsIds = csvRecords
          .filter(csvRow => Boolean(csvRow))
          .map(csvRow => Number(csvRow.phish_id))


        // all IDs in DB set to online and that they aren't in CSV are going to online: false
        const idsGoesOffline = phishtankDbOnlineIds.filter(dbId => !phishtankCvsIds.includes(dbId))
        logger.trace(`IDs to set offline: ${idsGoesOffline}`)

        const currentIsoDate = currentDate.toISOString()
        idsGoesOffline.map(async id => {
          await connection
            .manager
            .getRepository(Phishtank)
            .update({ phishId: id }, { online: false, endTime: currentIsoDate })
        })

        const idsToInsert = phishtankCvsIds.filter(csvId => !phishtankDbIds.includes(csvId))
        logger.trace(`IDS to insert: ${idsToInsert}`)
        const rowsToInsert = csvRecords.filter(csvRow =>
          csvRow && idsToInsert.includes(Number(csvRow.phish_id)))

        logger.trace(`Rows to insert: ${rowsToInsert}`)
        const rowsIntoDb: Phishtank[] = rowsToInsert.map(row => ({
          phishId: Number(row.phish_id),
          url: row.url,
          phishDetailUrl: row.phish_detail_url,
          submissionTime: row.submission_time,
          verificationTime: row.verification_time,
          online: Boolean(row.online),
          target: row.target,
        }))

        // insert only 1k rows at once
        let rowsIntoDbSmaller = rowsIntoDb.splice(0, 1000)
        while (rowsIntoDbSmaller.length > 0) {
          logger.info(`Inserting ${rowsIntoDbSmaller.length} rows.`)
          await connection
            .manager
            .getRepository(Phishtank)
            .insert(rowsIntoDbSmaller)

          rowsIntoDbSmaller = rowsIntoDb.splice(0, 1000)
        }

        logger.info(`Updating table lastUpdated to ${currentIsoDate}.`)
        await connection
          .manager
          .getRepository(LastUpdated)
          .update({ tableName: TableName.PHISHTANK }, { lastUpdated: currentIsoDate })


        logger.info('Done.')
      }
    }
  })
}

if (require.main === module) {
  run()
}
