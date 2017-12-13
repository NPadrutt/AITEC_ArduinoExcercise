/*
  Use this script for preparing or resetting the database.
  Just Copy&Paste the whole thing and run it in a SQL window.
*/

--
-- Create database for webchat
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
-- Create tables for webchat
--
CREATE TABLE `ringings` (
  `id`        int(10)        unsigned NOT NULL auto_increment,
  `uid`        varchar(60)        ,
  `timeStamp`        timestamp      NOT NULL default CURRENT_TIMESTAMP,
  `base64Image`  mediumtext    NOT NULL,
  `reply`      varchar(2048)  NOT NULL,

  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

