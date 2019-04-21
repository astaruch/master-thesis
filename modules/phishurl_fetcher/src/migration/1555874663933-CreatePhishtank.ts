import { MigrationInterface, QueryRunner, Table } from 'typeorm'


export class CreatePhishtank1555874663933 implements MigrationInterface {
  public async up(queryRunner: QueryRunner): Promise<void> {
    await queryRunner.createTable(new Table({
      name: 'phishtank',
      columns: [
        {
          name: 'id',
          type: 'int',
          isPrimary: true,
          isGenerated: true,
        },
        {
          name: 'phish_id',
          type: 'int',
        },
        {
          name: 'phish_detail_url',
          type: 'varchar',
        },
        {
          name: 'url',
          type: 'varchar',
        },
        {
          name: 'submission_time',
          type: 'timestamp with time zone',
        },
        {
          name: 'verification_time',
          type: 'timestamp with time zone',
        },
        {
          name: 'online',
          type: 'boolean',
        },
        {
          name: 'target',
          type: 'varchar',
        },
        {
          name: 'end_time',
          type: 'timestamp with time zone',
          isNullable: true,
        },
      ],
    }), true)
  }

  public async down(queryRunner: QueryRunner): Promise<void> {
    await queryRunner.dropTable('phishtank')
  }
}
