-- Understanding the xID
-- M (MID): link to MASTER table
-- L (LID): link to LABELS table
-- S (SID): link to SOURCES table
-- D (DID): link to DRUGS table
-- M (MID): link to MOLS table
-- R (RID): link to ROUTES table
-- IA (IAID): link to INTERACTIONS table
-- IAK (IAKID): link to INTERACTION_KNOWLEDGE table
-- BIB_ID: link to BIBLIOGRAPHY table

-- In one to many tables
-- MASTER_xID: is used to link one to many: 1 MASTER_xID is many xID
-- AID_MASTER_LID: is used to link one authorization text to many translations in LABELS_LINK


CREATE TABLE IF NOT EXISTS MASTER (
  DID      INTEGER PRIMARY KEY,
  UID1     varchar(50) not null,
  UID2     varchar(50),
  UID3     varchar(50),
  OLD_UID  varchar(50),
  SID      integer
);

CREATE TABLE IF NOT EXISTS SOURCES (
  SID             INTEGER PRIMARY KEY,
  DATABASE_UID    varchar(20) not null,
  MASTER_LID      integer,
  LANG            varchar(5),
  WEB             varchar(250),
  COPYRIGHT       varchar(500),
  DATE            date,
  DRUGS_VERSION   varchar(10),
  AUTHORS         varchar(200),
  VERSION         varchar(10),
  PROVIDER        varchar(200),
  WEBLINK         varchar(500),
  DRUG_UID_NAME   varchar(50),
  ATC             bool default true,
  INTERACTIONS    bool default true,
  COMPLEMENTARY_WEBSITE  varchar(200),
  PACK_MAIN_CODE_NAME    varchar(50),
  MOL_LINK_COMPLETION    integer default 0,
  AUTHOR_COMMENTS        varchar(10000),
  DRUGS_NAME_CONSTRUCTOR varchar(200),
  FREEMEDFORMS_COMPTA_VERSION varchar(10),
  WEBPORTAL_COMPTA_VERSION    varchar(10)
);

-- No PK till this table is a linkage table 1 -> N
CREATE TABLE IF NOT EXISTS LABELS_LINK (
  MASTER_LID    integer,
  LID           integer,
  UNIQUE(MASTER_LID, LID)
);

CREATE TABLE IF NOT EXISTS LABELS (
  LID   INTEGER PRIMARY KEY,
  LANG  varchar(5),
  LABEL varchar(250)
);

CREATE TABLE IF NOT EXISTS DRUGS (
  ID        INTEGER PRIMARY KEY,
  DID       integer,
  SID       integer,
  NAME      varchar(200),
  ATC_ID    integer,
  STRENGTH  varchar(40),
  VALID     boolean,
  MARKETED  boolean,
  AID_MASTER_LID  integer,
  LINK_SPC  varchar(500),
  EXTRA_XML varchar(5000)
);

-- TODO: update strength and dose to real
CREATE TABLE IF NOT EXISTS COMPOSITION (
  DID           integer,
  MID           integer,
  STRENGTH      varchar(25),
  STRENGTH_NID  integer,
  DOSE_REF      varchar(25),
  DOSE_REF_NID  integer,
  NATURE        varchar(2),
  LK_NATURE     integer
);

CREATE TABLE IF NOT EXISTS MOLS (
  MID       INTEGER PRIMARY KEY,
  SID       integer,
  NAME      varchar(150),
  WWW       varchar(200)
);

CREATE TABLE IF NOT EXISTS UNITS (
  NID     INTEGER PRIMARY KEY,
  VALUE   varchar(20)
);

-- one to N (SID is remembered for some scripts features)
CREATE TABLE IF NOT EXISTS LK_MOL_ATC (
  MID     integer NOT NULL,
  ATC_ID  integer NOT NULL,
  SID     integer NOT NULL,
  UNIQUE(MID, ATC_ID)
);

-- TP IMPROVE
CREATE TABLE IF NOT EXISTS PACKAGING (
  DID integer not null,
  SID integer,
  PACKAGE_UID int(20) NOT NULL,
  LABEL varchar(500) NOT NULL,
  STATUS varchar(1),
  MARKETING int(1) NOT NULL DEFAULT 1,
  DATE varchar(25),
  OPTIONAL_CODE int(20)
);

-- 1 -> N
CREATE TABLE IF NOT EXISTS DRUG_ROUTES  (
  DID  integer NOT NULL,
  RID  integer NOT NULL,
  UNIQUE(DID,RID)
);

-- 1 -> N
CREATE TABLE IF NOT EXISTS DRUG_FORMS (
  DID           integer NOT NULL,
  MASTER_LID    integer NOT NULL,
  UNIQUE(DID,MASTER_LID)
);

CREATE TABLE IF NOT EXISTS ROUTES (
  RID           INTEGER PRIMARY KEY,
  MASTER_LID    integer,
  IS_SYSTEMIC   integer,
  UNIQUE(RID, MASTER_LID)
);
-- IS_SYSTEMIC:
--  0 : non systemic
--  1 : fully systemic
--  2 : moderate systemic passage
--  3 : weak systemic passage
--  4 : depending on INN (for future usage)

CREATE TABLE IF NOT EXISTS SEARCH_ENGINES (
  ID integer primary key,
  LABEL varchar(25),
  URL varchar(1000)
);

DELETE FROM SEARCH_ENGINES;
INSERT INTO SEARCH_ENGINES VALUES (NULL, "WHO ATC description", "http://www.whocc.no/atc_ddd_index/?&code=[[ONE_ATC_CODE]]&showdescription=yes");
INSERT INTO SEARCH_ENGINES VALUES (NULL, "Search drug name @ NIH", "http://vsearch.nlm.nih.gov/vivisimo/cgi-bin/query-meta?v%3Aproject=medlineplus&query=[[DRUG_NAME]]&x=0&y=0");
INSERT INTO SEARCH_ENGINES VALUES (NULL, "Search INN name @ NIH", "http://vsearch.nlm.nih.gov/vivisimo/cgi-bin/query-meta?v%3Aproject=medlineplus&query=[[ONE_ATC_CODE]]&x=0&y=0");

CREATE TABLE IF NOT EXISTS DB_SCHEMA_VERSION (
  ID INTEGER PRIMARY KEY,
  VERSION varchar(10),
  DATE  date,
  COMMENT varchar(500)
);

CREATE TABLE IF NOT EXISTS INTERACTIONS (
  IAID INTEGER PRIMARY KEY,
  ATC_ID1 integer NOT NULL ,
  ATC_ID2 integer NOT NULL
 );

-- one to one
CREATE TABLE IF NOT EXISTS IA_IAK (
  IAID   integer,
  IAKID  integer
 );

CREATE TABLE IF NOT EXISTS IAKNOWLEDGE (
  IAKID               INTEGER PRIMARY KEY,
  TYPE                varchar(5),
  RISK_MASTER_LID     integer,
  MAN_MASTER_LID      integer,
  BIB_MASTER_ID       integer,
  WWW                 varchar(100)
);


-- Generic ATC table (more than 5000 ATC codes are known)
-- Codes ID > 100 000 are interacting molecule names without ATC
-- Codes ID > 200 000 are interactions classes denomination
CREATE TABLE IF NOT EXISTS ATC (
  ATC_ID INTEGER PRIMARY KEY,
  CODE VARCHAR(7) NULL
);

CREATE TABLE IF NOT EXISTS ATC_LABELS (
  ATC_ID        integer,
  MASTER_LID    integer
);

-- Add IAM classes tree (one class can contains multiple INN)
CREATE TABLE IF NOT EXISTS IAM_TREE (
  ID_CLASS       integer not null,
  ID_ATC         integer not null,
  BIB_MASTER_ID  integer default -1
);

-- one to N
CREATE TABLE IF NOT EXISTS BIBLIOGRAPHY_LINKS (
  BIB_MASTER_ID integer,
  BIB_ID        integer,
  UNIQUE(BIB_MASTER_ID, BIB_ID)
);

-- If url is too long please consider to use tinyurl.com to create a smaller one
CREATE TABLE IF NOT EXISTS BIBLIOGRAPHY (
  BIB_ID    INTEGER PRIMARY KEY,
  TYPE      varchar(20),
  LINK      varchar(200),
  TEXTUAL_REFERENCE varchar(1000),
  ABSTRACT  varchar(4000),
  EXPLANATION varchar(4000)
);

INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.3.0","2010-03-01","Translated all fields");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.4.0","2010-03-15","DRUGS table : Adding GLOBAL_STRENGTH");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.4.0","2010-06-01","Adding the information table");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.4.4","2010-07-16","Adding LK_MOL_ATC table");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.4.4","2010-07-18","INFORMATIONS table : Adding the COMPLEMENTARY_WEBSITE");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.4.4","2010-07-22","COMPOSITION table : Adding ATC Molecule code");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.4.4","2010-07-22","Adding DB_SCHEMA_VERSION table");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.5.0","2010-09-22","Replacing UID from int to varchar");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.5.0","2010-09-23","Adding molecule links completion percent to INFORMATION table");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.5.2","2010-09-23","Adding routes tables and link table");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.5.5","2011-01-10","Totally redefined schema");

----
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.0.8","2009-01-01","Interactions: First version of the database");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.3.0","2010-03-02","Interactions: First SVN publication");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.4.4","2010-07-02","Interactions: Adding the ATC table");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.4.4","2010-07-16","Interactions: Adding the IAM_TREE table : 1 interacting class <-> N ATC");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.4.4","2010-07-22","Interactions: Adding INTERACTIONS and INTERACTION_KNOWLEDGE tables");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.4.4","2010-07-22","Interactions: Removing IAM_DENOMINATION and IAM_IMPORT tables");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.4.4","2010-07-22","Interactions: Adding DB_SCHEMA_VERSION table");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.4.4","2010-07-22","Interactions: First english translations of INTERACTION_KNOWLEDGE are available");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.5.2","2010-12-10","Interactions: Addind REFERENCES_LINK to knowledge");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.5.2","2010-14-10","Interactions: Addind SOURCES_LINK to IAM_TREE");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.5.2","2010-14-10","Interactions: Addind SOURCES table");
INSERT INTO DB_SCHEMA_VERSION VALUES (NULL,"0.5.5","2011-01-10","Interactions: Totally redefined schema");


-- Some queries

-- Get all drugs information (without composition) // SID can be retrived from the SOURCES table
-- SELECT DRUGS.* FROM MASTER
-- JOIN DRUGS ON DRUGS.DID=MASTER.DID
-- WHERE SID=1;

-- Get all drugs source labels
-- SELECT LABELS.LANG, LABELS.LABEL
-- FROM LABELS
-- JOIN SOURCES ON SOURCES.MASTER_LID=LABELS_LINK.MASTER_LID
-- JOIN LABELS_LINK ON LABELS_LINK.LID=LABELS.LID
-- WHERE SOURCES.DATABASE_UID="AFSSAPS_FR";

-- Get all route labels
-- SELECT LABELS.LANG, LABELS.LABEL
-- FROM LABELS
-- JOIN ROUTES ON ROUTES.MASTER_LID=LABELS_LINK.MASTER_LID
-- JOIN LABELS_LINK ON LABELS_LINK.LID=LABELS.LID
-- WHERE ROUTES.RID=3;

-- Get drug routes labels
-- SELECT LABELS.LANG, LABELS.LABEL
-- FROM DRUG_ROUTES
-- JOIN ROUTES ON ROUTES.RID=DRUG_ROUTES.RID
-- JOIN LABELS_LINK ON LABELS_LINK.MASTER_LID=ROUTES.MASTER_LID
-- JOIN LABELS ON LABELS_LINK.LID=LABELS.LID
-- WHERE DRUG_ROUTES.DID=3;

-- Get the RID from a route label
-- SELECT ROUTES.RID
-- FROM LABELS
-- JOIN LABELS_LINK ON LABELS_LINK.LID=LABELS.LID
-- JOIN ROUTES ON ROUTES.MASTER_LID=LABELS_LINK.MASTER_LID
-- WHERE LABELS.LABEL="intralymphatisch";

-- Get ATC ID, CODE, LABEL for one specific language
-- SELECT ATC.ATC_ID, ATC.CODE, LABELS.LABEL
-- FROM ATC
-- JOIN ATC_LABELS ON ATC_LABELS.ATC_ID=ATC.ATC_ID
-- JOIN LABELS_LINK ON LABELS_LINK.MASTER_LID=ATC_LABELS.MASTER_LID
-- JOIN LABELS ON LABELS_LINK.LID=LABELS.LID
-- WHERE LABELS.LANG="fr" AND ATC.ATC_ID=100;
-- Search a specific label
-- SELECT ATC.ATC_ID, ATC.CODE, LABELS.LABEL
-- FROM ATC
-- JOIN ATC_LABELS ON ATC_LABELS.ATC_ID=ATC.ATC_ID
-- JOIN LABELS_LINK ON LABELS_LINK.MASTER_LID=ATC_LABELS.MASTER_LID
-- JOIN LABELS ON LABELS_LINK.LID=LABELS.LID
-- WHERE LABELS.LANG="fr" AND LABELS.LABEL='ESOMEPRAZOLE';

-- Get risk infos about interactions
-- SELECT INTERACTIONS.IAID, INTERACTIONS.ATC_ID1, INTERACTIONS.ATC_ID2, LABELS.LANG, LABELS.LABEL
-- FROM INTERACTIONS
-- JOIN IA_IAK ON IA_IAK.IAID=INTERACTIONS.IAID
-- JOIN IAKNOWLEDGE ON IAKNOWLEDGE.IAKID=IA_IAK.IAKID
-- JOIN LABELS_LINK ON LABELS_LINK.MASTER_LID=IAKNOWLEDGE.RISK_MASTER_LID
-- JOIN LABELS ON LABELS_LINK.LID=LABELS.LID
-- WHERE INTERACTIONS.IAID=1;
-- Get management infos about interactions
-- SELECT INTERACTIONS.IAID, INTERACTIONS.ATC_ID1, INTERACTIONS.ATC_ID2, LABELS.LANG, LABELS.LABEL
-- FROM INTERACTIONS
-- JOIN IA_IAK ON IA_IAK.IAID=INTERACTIONS.IAID
-- JOIN IAKNOWLEDGE ON IAKNOWLEDGE.IAKID=IA_IAK.IAKID
-- JOIN LABELS_LINK ON LABELS_LINK.MASTER_LID=IAKNOWLEDGE.MAN_MASTER_LID
-- JOIN LABELS ON LABELS_LINK.LID=LABELS.LID
-- WHERE INTERACTIONS.IAID=1;

