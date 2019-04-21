import 'reflect-metadata'
import { createConnection } from 'typeorm'
import { Phishtank } from './entity/Phishtank'


createConnection().then(async connection => {
  console.log('Inserting a new record into the database...')
  // 6014538,
  // https://outletdeprimaveira.com/index.php?produto=489466851,
  // http://www.phishtank.com/phish_detail.php?phish_id=6014538,
  // 2019-04-17T20:36:17+00:00,
  // yes,
  // 2019-04-17T20:37:05+00:00,
  // yes,
  // Other

  const phishtankRecord = new Phishtank()
  phishtankRecord.phishId = 6014538
  phishtankRecord.url = 'https://outletdeprimaveira.com/index.php?produto=489466851'
  phishtankRecord.phishDetailUrl = 'http://www.phishtank.com/phish_detail.php?phish_id=6014538'
  phishtankRecord.submssionTime = '2019-04-17T20:36:17+00:00'
  phishtankRecord.verificationTime = '2019-04-17T20:37:05+00:00'
  phishtankRecord.online = true
  phishtankRecord.target = 'Other'


  await connection.manager.save(phishtankRecord)
  console.log(`Saved a new user with id: ${phishtankRecord.id}`)

  console.log('Loading users from the database...')
  const records = await connection.manager.find(Phishtank)
  console.log('Loaded users: ', records)

  console.log('Here you can setup and run express/koa/any other framework.')
}).catch(error => console.log(error))
