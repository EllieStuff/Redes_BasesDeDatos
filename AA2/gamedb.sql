-- phpMyAdmin SQL Dump
-- version 4.6.6deb5
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Generation Time: Jan 22, 2020 at 05:14 PM
-- Server version: 5.7.28-0ubuntu0.18.04.4
-- PHP Version: 7.2.24-0ubuntu0.18.04.2

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `gamedb`
--

-- --------------------------------------------------------

--
-- Table structure for table `Enemies`
--

CREATE TABLE `Enemies` (
  `Enemy_ID` int(11) NOT NULL,
  `Sprite_Name` varchar(11) NOT NULL,
  `Type` varchar(11) NOT NULL,
  `Gem_FK` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `Enemies`
--

INSERT INTO `Enemies` (`Enemy_ID`, `Sprite_Name`, `Type`, `Gem_FK`) VALUES
(1, 'default', 'soldier', 1),
(2, 'default', 'soldier', 2),
(3, 'sniper', 'sniper', 3),
(4, 'sniper', 'sniper', 4),
(5, 'boss', 'boss', 5);

-- --------------------------------------------------------

--
-- Table structure for table `Gems`
--

CREATE TABLE `Gems` (
  `Gem_ID` int(11) NOT NULL,
  `Puntuacion` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `Gems`
--

INSERT INTO `Gems` (`Gem_ID`, `Puntuacion`) VALUES
(1, 1),
(2, 2),
(3, 3),
(4, 4),
(5, 5);

-- --------------------------------------------------------

--
-- Table structure for table `Players`
--

CREATE TABLE `Players` (
  `PlayerID` int(11) NOT NULL,
  `PlayerName` varchar(20) NOT NULL,
  `PlayerPassword` varchar(20) NOT NULL,
  `LastPrizeTime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `Players`
--

INSERT INTO `Players` (`PlayerID`, `PlayerName`, `PlayerPassword`, `LastPrizeTime`) VALUES
(2346, 'Test1', '3245', '2020-01-22 12:00:41'),
(2347, 'player2', '1564', '2020-01-04 19:25:19'),
(2349, 'Joan', '123', '2020-01-21 17:15:43'),
(2350, 'Miliviu', '111', '2020-01-22 17:11:11');

-- --------------------------------------------------------

--
-- Table structure for table `Players_Gems_FK`
--

CREATE TABLE `Players_Gems_FK` (
  `Player_FK` int(11) NOT NULL,
  `Gem_FK` int(11) NOT NULL,
  `Amount` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `Enemies`
--
ALTER TABLE `Enemies`
  ADD PRIMARY KEY (`Enemy_ID`),
  ADD KEY `FK_ENEMY_GEM` (`Gem_FK`);

--
-- Indexes for table `Gems`
--
ALTER TABLE `Gems`
  ADD PRIMARY KEY (`Gem_ID`);

--
-- Indexes for table `Players`
--
ALTER TABLE `Players`
  ADD PRIMARY KEY (`PlayerID`);

--
-- Indexes for table `Players_Gems_FK`
--
ALTER TABLE `Players_Gems_FK`
  ADD KEY `FK_PLAYER_DELETE` (`Player_FK`),
  ADD KEY `FK_GEM_DELETE` (`Gem_FK`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `Enemies`
--
ALTER TABLE `Enemies`
  MODIFY `Enemy_ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;
--
-- AUTO_INCREMENT for table `Gems`
--
ALTER TABLE `Gems`
  MODIFY `Gem_ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;
--
-- AUTO_INCREMENT for table `Players`
--
ALTER TABLE `Players`
  MODIFY `PlayerID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2351;
--
-- Constraints for dumped tables
--

--
-- Constraints for table `Enemies`
--
ALTER TABLE `Enemies`
  ADD CONSTRAINT `FK_ENEMY_GEM` FOREIGN KEY (`Gem_FK`) REFERENCES `Gems` (`Gem_ID`) ON DELETE CASCADE;

--
-- Constraints for table `Players_Gems_FK`
--
ALTER TABLE `Players_Gems_FK`
  ADD CONSTRAINT `FK_GEM_DELETE` FOREIGN KEY (`Gem_FK`) REFERENCES `Gems` (`Gem_ID`) ON DELETE CASCADE ON UPDATE NO ACTION,
  ADD CONSTRAINT `FK_PLAYER_DELETE` FOREIGN KEY (`Player_FK`) REFERENCES `Players` (`PlayerID`) ON DELETE CASCADE ON UPDATE NO ACTION;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
