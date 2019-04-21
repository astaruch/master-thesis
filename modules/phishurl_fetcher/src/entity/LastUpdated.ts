/* eslint-disable import/group-exports */
import { Entity, PrimaryGeneratedColumn, Column } from 'typeorm'

export enum TableName {
  PHISHTANK = 'phishtank'
}

@Entity()
export class LastUpdated {
  @PrimaryGeneratedColumn()
  public id: number

  /**
   * The name of table.
   */
  @Column({
    type: 'enum',
    enum: TableName,
    name: 'table_name',
  })
  public tableName: TableName

  /**
   * The date and time at which was table last updated.
   */
  @Column({ type: 'timestamp with time zone', name: 'last_updated' })
  public lastUpdated: string
}
