import { MigrationInterface, QueryRunner, Table } from 'typeorm'
import { TableName } from '../entity/LastUpdated'

const name = 'last_updated'

export class CreateLastUpdated1555897004589 implements MigrationInterface {
  public async up(queryRunner: QueryRunner): Promise<void> {
    await queryRunner.createTable(new Table({
      name,
      columns: [
        {
          name: 'id',
          type: 'int',
          isPrimary: true,
          isGenerated: true,
        },
        {
          name: 'table_name',
          type: 'enum',
          enum: [TableName.PHISHTANK],
        },
        {
          name: 'last_updated',
          type: 'timestamp with time zone',
        },
      ],
    }), true)
  }

  public async down(queryRunner: QueryRunner): Promise<void> {
    await queryRunner.dropTable(name)
  }
}
