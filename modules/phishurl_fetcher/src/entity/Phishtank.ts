import { Entity, PrimaryGeneratedColumn, Column } from 'typeorm'

@Entity()
export class Phishtank {
  @PrimaryGeneratedColumn()
  public id?: number

  /**
   * The ID number by which Phishtank refers to a phish submission.
   * All data in PhishTank is tied to this ID. This will always be a positive integer
   */
  @Column({ name: 'phish_id', unique: true })
  public phishId: number

  /**
   * PhishTank detail url for the phish, where you can view data about
   * the phish, including a screenshot and the community votes.
   */
  @Column({ name: 'phish_detail_url' })
  public phishDetailUrl: string

  /**
   * The phish URL. This is always a string.
   */
  @Column()
  public url: string

  /**
   * The date and time at which this phish was reported to Phishtank.
   * This is an ISO 8601 formatted date.
   */
  @Column({ type: 'timestamp with time zone', name: 'submission_time' })
  public submissionTime: string

  /**
   * The date and time at which the phish was verified as valid by our community.
   * This is an ISO 8601 formatted date.
   */
  @Column({ type: 'timestamp with time zone', name: 'verification_time' })
  public verificationTime: string

  /**
   *  Whether or not the phish is online and operational.
   */
  @Column()
  public online: boolean

  /**
   * The name of the company or brand the phish is impersonating, if it's known.
   */
  @Column()
  public target: string

  /**
   * The date and time at which this phish was taken down from the internet.
   */
  @Column({ type: 'timestamp with time zone', name: 'end_time', nullable: true })
  public endTime?: string
}
