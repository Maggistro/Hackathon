package commands;

public enum Command {
	START, STOP, FOLLOW, RESET, SPEED_UP, SPEED_DOWN, INFO, STATUS;

	/**
	 * This method checks if a command of this enum is equals to the given name
	 * 
	 * @param commandName
	 *            the name of a command which should be in this enum.
	 * @return the enum for the name or <code>null</code> if the name dosn't
	 *         match with on of this enum names or is invalid [empty, whitspace, "null"].
	 */
	public static Command getCommandFromName(String commandName) {
		Command result = null;

		if (commandName != null && !"".equals(commandName)
				&& !" ".equals(commandName) && !"null".equals(commandName)) {
			Command[] commands = Command.values();
			int index = commands.length - 1; // Index 0
			String searchedCommandName = commandName.toUpperCase();

			do {
				Command comparedCommand = commands[index];

				if (comparedCommand.name().equals(searchedCommandName)) {
					result = comparedCommand;
				}

				index--;
			} while (index == 0 || result != null);
		}

		return result;
	}
}
