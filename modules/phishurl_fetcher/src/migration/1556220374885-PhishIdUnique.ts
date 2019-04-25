import { MigrationInterface, QueryRunner } from 'typeorm'
import { TableUnique } from 'typeorm/schema-builder/table/TableUnique'

const unique = new TableUnique({ columnNames: ['phish_id'] })

export class PhishIdUnique1556220374885 implements MigrationInterface {
  public async up(queryRunner: QueryRunner): Promise<void> {
    // delete duplicate rows
    await queryRunner.query(`DELETE
      FROM
        phishtank a
          USING phishtank b
      WHERE
        a.id > b.id
        AND a.phish_id = b.phish_id;`)
    await queryRunner.createUniqueConstraint('phishtank', unique)
  }

  public async down(queryRunner: QueryRunner): Promise<void> {
    await queryRunner.dropUniqueConstraint('phishtank', unique)
  }
}
