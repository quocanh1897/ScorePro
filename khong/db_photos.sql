-- phpMyAdmin SQL Dump
-- version 4.0.4
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Dec 07, 2015 at 10:20 AM
-- Server version: 5.6.12-log
-- PHP Version: 5.4.16

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `db_photos`
--
CREATE DATABASE IF NOT EXISTS `db_photos` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci;
USE `db_photos`;

-- --------------------------------------------------------

--
-- Table structure for table `tbl_images`
--

CREATE TABLE IF NOT EXISTS `tbl_images` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `imgname` varchar(250) NOT NULL,
  `imgtype` varchar(100) NOT NULL,
  `imgsize` varchar(100) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=13 ;

--
-- Dumping data for table `tbl_images`
--

INSERT INTO `tbl_images` (`id`, `imgname`, `imgtype`, `imgsize`) VALUES
(1, 'Nature01 (4).jpg', 'image/jpeg', '1023419'),
(2, '14506Cities  (42).jpg', 'image/jpeg', '1554477'),
(3, '26859Cities  (43).jpg', 'image/jpeg', '1061404'),
(4, '329352Cities  (44).jpg', 'image/jpeg', '1044382'),
(5, '06311Cities  (46).jpg', 'image/jpeg', '1082536'),
(6, '14436Cities  (47).jpg', 'image/jpeg', '1045626'),
(7, '210493Cities  (48).jpg', 'image/jpeg', '1080080'),
(8, '314834Cities  (50).jpg', 'image/jpeg', '1077654'),
(9, '022521Nature01 (1).jpg', 'image/jpeg', '1311473'),
(10, '1830Nature01 (3).jpg', 'image/jpeg', '1310961'),
(12, '314060Nature01 (5).jpg', 'image/jpeg', '800313');

-- --------------------------------------------------------

--
-- Table structure for table `tbl_login`
--

CREATE TABLE IF NOT EXISTS `tbl_login` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(100) NOT NULL,
  `password` varchar(100) NOT NULL,
  `type` varchar(100) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

--
-- Dumping data for table `tbl_login`
--

INSERT INTO `tbl_login` (`id`, `username`, `password`, `type`) VALUES
(1, 'admin', 'admin', 'admin');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
