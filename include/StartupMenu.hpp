/**
 * @file StartupMenu.hpp
 * @brief Declares the StartupMenu class for initializing and managing the game's main menu.
 *
 * Handles menu display, player and company setup, configuration persistence,
 * and launching the main game loop.
 */

#pragma once

/**
 * @class StartupMenu
 * @brief Manages the initial menu and configuration phase before the main game starts.
 *
 * Provides user interaction for choosing game settings, loading or saving configuration,
 * and starting the gameplay session.
 */
class StartupMenu
{
public:
    /** @brief Default constructor. */
    StartupMenu() = default;

    /**
     * @brief Starts the main menu loop.
     *
     * Displays menu options for starting the game, changing settings,
     * or exiting the application. Handles configuration loading/saving
     * and launches the game once setup is complete.
     *
     * @return 0 on successful completion or user exit.
     */
    int StartMenuLoop();
};
