/*
  Use this script for preparing or resetting the database.
  Just Copy&Paste the whole thing and run it in a SQL window.
*/

--
-- Create database for doorkeeper
--
DROP DATABASE IF EXISTS `doorkeeper`;
CREATE DATABASE `doorkeeper` /*!40100 DEFAULT CHARACTER SET utf8 */;

--
-- Create webchat user
--
DROP USER IF EXISTS `bouncer`@`localhost`;
CREATE USER `bouncer`@`localhost` IDENTIFIED BY 'Dakommstnichtrein!';

--
-- Set user privileges
--
GRANT SELECT, INSERT, UPDATE, DELETE ON doorkeeper.* TO 'bouncer'@'localhost';

--
-- Set current database
--
USE `doorkeeper`;

--
-- Create tables for doorkeeper
--
CREATE TABLE `ringings` (
  `id`            int(10)       unsigned NOT NULL auto_increment,
  `device_ip`     varchar(64)   NOT NULL,
  `device_agent`  varchar(128)  NOT NULL,
  `device_uuid`   varchar(64)   NOT NULL,
  `base64Image`   mediumtext,
  `reply`         varchar(256),
  `created`       timestamp     NOT NULL default CURRENT_TIMESTAMP,

  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

