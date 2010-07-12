package mccf;

import java.text.FieldPosition;
import java.text.MessageFormat;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.MissingResourceException;
import java.util.PropertyResourceBundle;

/**
 * <p>This class helps with internationalization.<p>
 * <p>It utilises {@link PropertyResourceBundle} and {@link MessageFormat} caching created objects statically.</p>
 * <p>It supports lookup of {@link PropertyResourceBundle}s by the {@link Class#getCanonicalName() canonical name} of a given object or class.</p>
 * @see PropertyResourceBundle
 * @see MessageFormat
 */
public class MessageHelper {
	private final static Map<String, Map<Locale, PropertyResourceBundle>> bundleKey2Locale2Bundle = new HashMap<String, Map<Locale, PropertyResourceBundle>>();
	private final static Map<String, Map<String, Map<Locale, MessageFormat>>> bundleKey2MessageKey2Locale2MessageFormat = new HashMap<String, Map<String, Map<Locale, MessageFormat>>>();
	
	/**
	 * <p>Get the message formatted with the given arguments.</p>
	 * @param bundleKey The key (and base name) for the {@link PropertyResourceBundle} to use.
	 * @param messageKey The key of the message in the property resource bundle.
	 * @param locale The locale to use, if it is {@code null} then the {@link Locale#getDefault() default locale} will be used.
	 * @param arguments The arguments to format the message with, if it is {@code null} or empty no formatting will be done.
	 * @return The formatted message.
	 * @throws NullPointerException If {@code bundleKey} or {@code messageKey} are {@code null}.
	 * @throws IllegalArgumentException If arguments is neither {@code null} nor empty and the message is no valid {@code MessageFormat} pattern.
	 * @throws MissingResourceException If no {@link PropertyResourceBundle} with base name {@code bundleKey} can be found.
	 */
	public static String get(final String bundleKey, final String messageKey, Locale locale, final Object... arguments) throws NullPointerException, IllegalArgumentException, MissingResourceException {
		if(bundleKey == null || messageKey == null)
			throw new NullPointerException();
		
		if(locale == null)
			locale = Locale.getDefault();
		
		if(arguments == null || arguments.length == 0)
			return getMessage(bundleKey, messageKey, locale);
		
		return format(bundleKey, messageKey, locale, arguments);
	}
	
	private static String format(final String bundleKey, final String messageKey, final Locale locale, final Object... arguments) throws IllegalArgumentException, MissingResourceException {
		assert bundleKey != null;
		assert messageKey != null;
		assert locale != null;

		MessageFormat messageFormat;
		StringBuffer result;
		FieldPosition pos;
		
		messageFormat = getMessageFormat(bundleKey, messageKey, locale);
		result = new StringBuffer();
		pos = new FieldPosition(0);
		messageFormat.format(arguments, result, pos);

		return result.toString();
	}
	
	private static MessageFormat getMessageFormat(final String bundleKey, final String messageKey, final Locale locale) throws IllegalArgumentException, MissingResourceException {
		assert bundleKey != null;
		assert messageKey != null;
		assert locale != null;

		Map<String, Map<Locale, MessageFormat>> messageKey2Locale2MessageFormat;
		Map<Locale, MessageFormat> locale2MessageFormat;
		MessageFormat messageFormat;
		
		messageKey2Locale2MessageFormat = bundleKey2MessageKey2Locale2MessageFormat.get(bundleKey);

		if(messageKey2Locale2MessageFormat == null) {
			messageKey2Locale2MessageFormat = new HashMap<String, Map<Locale, MessageFormat>>();
			bundleKey2MessageKey2Locale2MessageFormat.put(bundleKey, messageKey2Locale2MessageFormat);
		}
		
		locale2MessageFormat = messageKey2Locale2MessageFormat.get(messageKey);
		
		if(locale2MessageFormat == null) {
			locale2MessageFormat = new HashMap<Locale, MessageFormat>();
			messageKey2Locale2MessageFormat.put(messageKey, locale2MessageFormat);
		}
		
		messageFormat = locale2MessageFormat.get(locale);
		
		if(messageFormat == null) {
			String message;
			
			message = getMessage(bundleKey, messageKey, locale);
			messageFormat = new MessageFormat(message, locale);
			locale2MessageFormat.put(locale, messageFormat);
		}
		
		return messageFormat;
	}
	
	private static String getMessage(final String bundleKey, final String messageKey, final Locale locale) throws MissingResourceException {
		assert bundleKey != null;
		assert messageKey != null;
		assert locale != null;

		Map<Locale, PropertyResourceBundle> locale2Bundle;
		PropertyResourceBundle bundle;
		
		locale2Bundle = bundleKey2Locale2Bundle.get(bundleKey);
		
		if(locale2Bundle == null) {
			locale2Bundle = new HashMap<Locale, PropertyResourceBundle>();
			bundleKey2Locale2Bundle.put(bundleKey, locale2Bundle);
		}
		
		bundle = locale2Bundle.get(locale);
		
		if(bundle == null) {
			bundle = (PropertyResourceBundle) PropertyResourceBundle.getBundle(bundleKey, locale);
			locale2Bundle.put(locale, bundle);
		}
		
		return bundle.getString(messageKey);
	}
	
	/**
	 * <p>Get the message formatted with the given arguments using the {@link Locale#getDefault() default locale}.</p>
	 * @param bundleKey The key (and base name) for the {@link PropertyResourceBundle} to use.
	 * @param messageKey The key of the message in the property resource bundle.
	 * @param arguments The arguments to format the message with, if it is {@code null} or empty no formatting will be done.
	 * @return The formatted message.
	 * @throws NullPointerException If {@code bundleKey} or {@code messageKey} are {@code null}.
	 * @throws IllegalArgumentException If arguments is neither {@code null} nor empty and the message is no valid {@code MessageFormat} pattern.
	 * @throws MissingResourceException If no {@link PropertyResourceBundle} with base name {@code bundleKey} can be found.
	 */
	public static String get(final String bundleKey, final String messageKey, final Object... arguments) throws NullPointerException, IllegalArgumentException, MissingResourceException {
		return get(bundleKey, messageKey, null, arguments);
	}
	
	/**
	 * <p>Calls {@link #get(String, String, Locale, Object...)} with the {@link Class#getCanonicalName() canonical name} of the given class {@code c} as the {@code bundleKey}.</p>
	 * @param c The class to determine the {@code bundleKey} from.
	 * @param messageKey The key of the message in the property resource bundle.
	 * @param locale The locale to use, if it is {@code null} then the {@link Locale#getDefault() default locale} will be used.
	 * @param arguments The arguments to format the message with, if it is {@code null} or empty no formatting will be done.
	 * @return The formatted message.
	 * @throws NullPointerException If {@code c}, {@code bundleKey} or {@code messageKey} are {@code null}.
	 * @throws IllegalArgumentException If arguments is neither {@code null} nor empty and the message is no valid {@code MessageFormat} pattern.
	 * @throws MissingResourceException If no {@link PropertyResourceBundle} with base name {@code bundleKey} can be found.
	 */
	public static String get(final Class<?> c, final String messageKey, Locale locale, final Object... arguments) throws NullPointerException, IllegalArgumentException, MissingResourceException {
		if(c == null)
			throw new NullPointerException();

		return get(c.getCanonicalName(), messageKey, locale, arguments);
	}

	/**
	 * <p>Calls {@link #get(String, String, Object...)} with the {@link Class#getCanonicalName() canonical name} of the given class {@code c} as the {@code bundleKey}.</p>
	 * @param c The class to determine the {@code bundleKey} from.
	 * @param messageKey The key of the message in the property resource bundle.
	 * @param arguments The arguments to format the message with, if it is {@code null} or empty no formatting will be done.
	 * @return The formatted message.
	 * @throws NullPointerException If {@code c}, {@code bundleKey} or {@code messageKey} are {@code null}.
	 * @throws IllegalArgumentException If arguments is neither {@code null} nor empty and the message is no valid {@code MessageFormat} pattern.
	 * @throws MissingResourceException If no {@link PropertyResourceBundle} with base name {@code bundleKey} can be found.
	 */
	public static String get(final Class<?> c, final String key, final Object... arguments) throws NullPointerException, IllegalArgumentException, MissingResourceException {
		return get(c, key, null, arguments);
	}
	
	/**
	 * <p>Calls {@link #get(String, String, Locale, Object...)} with the {@link Class#getCanonicalName() canonical name} of the class of the object {@code o} as the {@code bundleKey}.</p>
	 * @param o The object to determine the {@code bundleKey} from.
	 * @param messageKey The key of the message in the property resource bundle.
	 * @param locale The locale to use, if it is {@code null} then the {@link Locale#getDefault() default locale} will be used.
	 * @param arguments The arguments to format the message with, if it is {@code null} or empty no formatting will be done.
	 * @return The formatted message.
	 * @throws NullPointerException If {@code o}, {@code bundleKey} or {@code messageKey} are {@code null}.
	 * @throws IllegalArgumentException If arguments is neither {@code null} nor empty and the message is no valid {@code MessageFormat} pattern.
	 * @throws MissingResourceException If no {@link PropertyResourceBundle} with base name {@code bundleKey} can be found.
	 */
	public static String get(final Object o, final String key, final Locale locale, final Object... arguments) throws NullPointerException, IllegalArgumentException, MissingResourceException {
		if(o == null)
			throw new NullPointerException();

		return get(o.getClass(), key, locale, arguments);
	}

	/**
	 * <p>Calls {@link #get(String, String, Object...)} with the {@link Class#getCanonicalName() canonical name} of the class of the object {@code o} as the {@code bundleKey}.</p>
	 * @param c The object to determine the {@code bundleKey} from.
	 * @param messageKey The key of the message in the property resource bundle.
	 * @param arguments The arguments to format the message with, if it is {@code null} or empty no formatting will be done.
	 * @return The formatted message.
	 * @throws NullPointerException If {@code o}, {@code bundleKey} or {@code messageKey} are {@code null}.
	 * @throws IllegalArgumentException If arguments is neither {@code null} nor empty and the message is no valid {@code MessageFormat} pattern.
	 * @throws MissingResourceException If no {@link PropertyResourceBundle} with base name {@code bundleKey} can be found.
	 */
	public static String get(final Object o, final String key, final Object... arguments) throws NullPointerException, IllegalArgumentException, MissingResourceException {
		return get(o, key, null, arguments);
	}
	
	/**
	 * Forget all cached resource bundles.
	 */
	public static void reset() {
		bundleKey2Locale2Bundle.clear();
		bundleKey2MessageKey2Locale2MessageFormat.clear();
	}
}
