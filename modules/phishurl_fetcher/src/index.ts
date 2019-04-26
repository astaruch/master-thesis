/* eslint-disable no-await-in-loop */
/* eslint-disable no-sync */
/* eslint-disable camelcase */
import 'reflect-metadata'
import * as fs from 'fs'
import * as path from 'path'
import { createConnection, Repository, getManager, EntityManager } from 'typeorm'
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

const getFilenames = (dir: string): string[] => {
  let files
  try {
    files = fs.readdirSync(dir)
  } catch (err) {
    logger.error(`No such directory: ${dir}`)
    return []
  }
  return files
}

const getLastUpdated = async (repository: Repository<LastUpdated>): Promise<Date> => {
  try {
    const row = await repository.findOneOrFail({ tableName: TableName.PHISHTANK })
    return row.lastUpdated
  } catch (err) {
    logger.warn('LastUpdate table doesn\'t exists. Inserting epoch time zero into database.')
    const date = new Date(0)
    await repository.insert({ tableName: TableName.PHISHTANK, lastUpdated: date.toISOString() })
    return date
  }
}

const getPhishtankCsvRecords = (fullpath: string): IPhishtankCsvRecordType[] => {
  const csvContent = fs.readFileSync(fullpath)
  return parse(csvContent.toString('utf8'), {
    columns: true,
    delimiter: ',',
  })
}

const setOfflineIds = (
  repository: Repository<Phishtank>,
  onlineDbIds: number[],
  newIds: number[],
  endTime: string,
): void => {
  const goingOffline = onlineDbIds.filter(id => !newIds.includes(id))
  goingOffline.map(async id => {
    await repository.update({ phishId: id }, { online: false, endTime })
  })
}

const insertPhishtankRecordsFromCsv = async (
  manager: EntityManager,
  csvRecords: IPhishtankCsvRecordType[],
  dbIds: number[],
): Promise<void> => {
  const idsToInsert = csvRecords
    .filter(row => row)
    .map(row => Number(row.phish_id))
    .filter(newId => !dbIds.includes(newId))

  const recordsToInsert = csvRecords
    .filter(row => row && idsToInsert.includes(Number(row.phish_id)))

  const newDbRecords: Phishtank[] = recordsToInsert.map(row => ({
    phishId: Number(row.phish_id),
    url: row.url,
    phishDetailUrl: row.phish_detail_url,
    submissionTime: row.submission_time,
    verificationTime: row.verification_time,
    online: Boolean(row.online),
    target: row.target,
  }))

  // insert only 1k rows at once (in transaction)
  manager.transaction(async transactionalEntityManager => {
    let recordsSplice = newDbRecords.splice(0, 1000)
    while (recordsSplice.length > 0) {
      logger.info(`Inserting ${recordsSplice.length} rows.`)
      try {
        await transactionalEntityManager.insert(Phishtank, recordsSplice)
      } catch (err) {
        logger.error('Can\'t perform insert')
        throw new Error(err)
      }
      recordsSplice = recordsSplice.splice(0, 1000)
    }
  })
}

const initializeFromPhishtank = (): void => {
  createConnection().then(async connection => {
    const dir = '../../../../master-thesis-phishtank-data/data/csv'
    const fulldir = path.join(__dirname, dir)
    const files = getFilenames(fulldir)
    const repositoryLastUpdated = await connection.manager.getRepository(LastUpdated)
    const repositoryPhishtank = await connection.manager.getRepository(Phishtank)
    for (let i = 0; i < files.length; i++) {
      const fullpath = path.join(fulldir, files[i])
      try {
        if (!fs.statSync(fullpath).isFile()) {
          logger.warn(`Skipping non-file: ${fullpath}`)
          continue
        }
      } catch (err) {
        logger.error(err)
        throw new Error()
      }

      // current date is same as csv filename
      // removing '.csv' extension
      const currentDate = new Date(files[i].substr(0, files[i].length - 4))
      const currentIsoDate = currentDate.toISOString()
      const lastDate = await getLastUpdated(repositoryLastUpdated)

      if (currentDate > lastDate) {
        logger.info(`Updating database with file: ${files[i]}`)
        const phishtankDb = await repositoryPhishtank
          .createQueryBuilder('phishtank')
          .getMany()

        const phishtankDbIds = phishtankDb
          .filter(dbRow => dbRow)
          .map(dbRow => dbRow.phishId)

        const phishtankDbOnlineIds = phishtankDb
          .filter(dbRow => dbRow && dbRow.online)
          .map(dbRow => dbRow.phishId)

        const phishtankCsv = getPhishtankCsvRecords(fullpath)

        const phishtankCvsIds = phishtankCsv
          .filter(csvRow => csvRow)
          .map(csvRow => Number(csvRow.phish_id))

        // If phish_id is in db (with online: true) and it's not in csv,
        // it's got offline right now.
        setOfflineIds(repositoryPhishtank, phishtankDbOnlineIds, phishtankCvsIds, currentIsoDate)

        // Insert all new records into database where phish_id isnt't already there
        await insertPhishtankRecordsFromCsv(getManager(), phishtankCsv, phishtankDbIds)

        // Update timestamp in database
        logger.info(`Updating table lastUpdated to ${currentIsoDate}.`)
        await repositoryLastUpdated
          .update({ tableName: TableName.PHISHTANK }, { lastUpdated: currentIsoDate })
      }
    }
  })
}

if (require.main === module) {
  initializeFromPhishtank()
}
