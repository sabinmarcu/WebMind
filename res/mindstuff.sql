-- phpMyAdmin SQL Dump
-- version 3.4.3.2
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Aug 05, 2011 at 09:40 AM
-- Server version: 5.5.14
-- PHP Version: 5.3.5

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `mindstuff`
--

-- --------------------------------------------------------

--
-- Table structure for table `auth`
--

CREATE TABLE IF NOT EXISTS `auth` (
  `doc` smallint(5) unsigned NOT NULL,
  `openid` varchar(256) CHARACTER SET ascii COLLATE ascii_bin NOT NULL,
  `write` tinyint(1) NOT NULL,
  PRIMARY KEY (`doc`,`openid`),
  KEY `doc` (`doc`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `docs`
--

CREATE TABLE IF NOT EXISTS `docs` (
  `id` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `title` varchar(128) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

--
-- Dumping data for table `docs`
--

INSERT INTO `docs` (`id`, `title`) VALUES
(1, 'Doc');

-- --------------------------------------------------------

--
-- Table structure for table `nodes`
--

CREATE TABLE IF NOT EXISTS `nodes` (
  `id` mediumint(8) unsigned NOT NULL AUTO_INCREMENT,
  `parent` mediumint(8) unsigned DEFAULT NULL,
  `doc` smallint(5) unsigned NOT NULL,
  `text` varchar(256) NOT NULL DEFAULT 'New Document',
  `x` smallint(4) NOT NULL DEFAULT '25',
  `y` smallint(4) NOT NULL DEFAULT '75',
  PRIMARY KEY (`id`),
  KEY `parent` (`parent`,`doc`),
  KEY `doc` (`doc`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=3 ;

--
-- Dumping data for table `nodes`
--

INSERT INTO `nodes` (`id`, `parent`, `doc`, `text`, `x`, `y`) VALUES
(1, NULL, 1, 'Main Node', 212, 113);

--
-- Constraints for dumped tables
--

--
-- Constraints for table `auth`
--
ALTER TABLE `auth`
  ADD CONSTRAINT `auth_ibfk_1` FOREIGN KEY (`doc`) REFERENCES `docs` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `nodes`
--
ALTER TABLE `nodes`
  ADD CONSTRAINT `nodes_ibfk_1` FOREIGN KEY (`parent`) REFERENCES `nodes` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `nodes_ibfk_2` FOREIGN KEY (`doc`) REFERENCES `docs` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
