//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, vJAXB 2.1.10 in JDK 6 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2011.01.19 at 11:06:42 AM CET 
//


package mccf.definition;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ChoiceReference complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ChoiceReference">
 *   &lt;complexContent>
 *     &lt;extension base="{urn:strike-devices:configuration}Reference">
 *       &lt;attribute name="option" use="required" type="{http://www.w3.org/2001/XMLSchema}int" />
 *     &lt;/extension>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ChoiceReference")
public class ChoiceReference
    extends Reference
{

    @XmlAttribute(required = true)
    protected int option;

    /**
     * Gets the value of the option property.
     * 
     */
    public int getOption() {
        return option;
    }

    /**
     * Sets the value of the option property.
     * 
     */
    public void setOption(int value) {
        this.option = value;
    }

}
