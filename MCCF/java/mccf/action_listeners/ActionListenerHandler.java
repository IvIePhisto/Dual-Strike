package mccf.action_listeners;

import java.awt.event.ActionListener;
import java.util.HashMap;
import java.util.Map;

import javax.swing.AbstractButton;

public class ActionListenerHandler {
	private final Map<String, ActionListener> actionListeners = new HashMap<String, ActionListener>();	
	public synchronized void registerActionListener(final String name, final ActionListener actionListener) {
		actionListeners.put(name, actionListener);
	}
	
	private synchronized ActionListener getActionListener(final String actionListenerName) {
		ActionListener actionListener;
		
		actionListener = actionListeners.get(actionListenerName);
		
		if(actionListener == null)
			throw new Error(String.format("Action listener \"%s\" not registered.", actionListenerName));
		
		return actionListener;
	}
	
	public synchronized void registerAction(final AbstractButton button, final String actionListenerName) {
		button.addActionListener(getActionListener(actionListenerName));
	}
}
