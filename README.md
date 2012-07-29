# `xupl`

`xupl - ˈsəpəl`

Equivalent structure to XML.

Can look like sentences or programming language text.

## The Goals ##

If you can write XML, you already know xupl.

Reduce the verbosity of XML.

Provide a more natural, readable, less verbose means of manually creating structured documents.

Similar to writing programming language text and leverage existing programming IDEs.

Facilitate creating Domain Specific Languages (DSLs).

## For example ##

### Here is a [xupl document](/nicerobot/xupl/tree/master/language/src/test/resources/output/note.xup) ###

    note:
      to: "Tove"
      from: "Jani"
      heading: "Reminder"
      body: "Don't forget me this weekend!"

### For [this XML](/nicerobot/xupl/tree/master/language/src/test/resources/note.xml) ###

    <note>
      <to>Tove</to>
      <from>Jani</from>
      <heading>Reminder</heading>
      <body>Don't forget me this weekend!</body>
    </note>

Not only is the xupl less verbose, it's far easier to read. This becomes much more evident when namespaces are used.

### xupl formatted to read like a sentence ###

    note: to: "Tove". from: "Jani". heading: "Reminder". body: "Don't forget me this weekend!".

### Alternately, {} can replace :. ###

    note {
      to {"Tove"}
      from {"Jani"}
      heading: "Reminder"
      body: "Don't forget me this weekend!"
    }

This enables tools capable of matching braces.

## Attributes ##

Attributes are specified as `name "value"` or `"value" name`.

e.g.

    a "/nicerobot/xupl/wiki/xupl" href:"xupl"

and

    a href "/nicerobot/xupl/wiki/xupl":"xupl"

and

    a href "/nicerobot/xupl/wiki/xupl"{"xupl"}

all produce:

    <a href="/nicerobot/xupl/wiki/xupl">xupl</a>

Numeric values, including floats, can omit surrounding quotes.

e.g.

    size 100 width, 100 height

produces:

    <size width="100" height="100" />

Attribute values can remain unquoted for native [types](/nicerobot/text-plain/wiki):

- `numbers`: decimal int, float, base 2,8,10,16,32, and 64 constants
- sizes: n`px` n`em` n`%`
- `URLs`
- `mailto:`
- `#`tags|rgb
- `@`names
- `~`durations
- `^`date
- `!`priority

## More Examples ##

### HTML Example ###

    html:
      head{title:"My xupl"}
      body:
        a "/nicerobot/xupl/wiki/xupl":"xupl"

This example demonstrates a couple points.

1. When using `tag:` syntax, EOF will close all open tags.
1. Certain tags have default attribute names. e.g. `a` tag's default attribute name is `href`.

### HTML Output ###

    <html>
      <head><title>My xupl</title></head>
      <body>
        <a href="/nicerobot/xupl/wiki/xupl">xupl</a>
      </body>
    </html>

---

### A [Metal](/nicerobot/metal) Control ([Flickr Query #cat](http://xmetal.googlecode.com/svn/trunk/metal-common/src/test/html/flickrquery.xhtml#cat)) ###

    control@metal "flickrquery" name {
      form@metal http://query.yahooapis.com/v1/public/yql action, "get" method {
        input@metal "q" name,
          "select * from flickr.photos.search where text='${queryText}' limit 10" value.
        input@metal "format" name, "json" value.
        input@metal "env" name,
          http://datatables.org/alltables.env value
      }
      control@metal "photo" name, "this.query.results.photo" data {
        a "photo-${parent.index}.${index}" id, http://flickr.com/photo.gne?id=${this.id} href {
          img "20%" width, "20%" height, src
            http://farm${this.farm}.static.flickr.com/${this.server}/${this.id}_${this.secret}.jpg
        }
      }
    }

Produces [this Metal Control](/nicerobot/metal/blob/master/metal-common/src/test/resources/controls/flickrquery.xmtl).

---

### xupl for a [Sequence Diagram](/nicerobot/xupl/blob/master/xupl/samples/sequence2.xup) ###

    sequence{
      actors{
        user "User"
        participant view ":View"
        participant controller ":Controller"
        participant model ":Model"
      }
      messages{
        user "request()" view
        view "handleEvent()" controller
        controller "queryInformation()" model
        controller "doValidation()" controller
        view "updateView()" controller
        user "notifyUser()" view
      }
    }

### The XML ###

    <sequence>
        <actors>
            <user id="User"/>
            <participant view=":View"/>
            <participant controller=":Controller"/>
            <participant entity=":Model"/>
        </actors>
        <messages>
            <user view="request()"/>
            <view controller="handleEvent()"/>
            <controller model="queryInformation()"/>
            <controller controller="doValidation()"/>
            <view controller="updateView()"/>
            <user view="notifyUser()"/>
        </messages>
    </sequence>

---

### For [this XML](/nicerobot/xupl/tree/master/language/src/test/resources/painting.xml) ###

    <painting>
      <img src="madonna.jpg" alt="Foligno Madonna, by Raphael"></img>
      <caption>This is Raphael's "Foligno" Madonna, painted in    
        <date>1511</date>-<date>1512</date>.
      </caption>
    </painting>

### [The xupl](/nicerobot/xupl/tree/master/language/src/test/resources/output/painting.xup) ###

    painting:
      img "madonna.jpg" src, "Foligno Madonna, by Raphael" alt
      caption:
        """This is Raphael's "Foligno" Madonna, painted in """
        date: "1511". "-"
        date: "1512". "."

---

### For [this XML](/nicerobot/xupl/tree/master/language/src/test/resources/ical.xml) ###

    <?xml version="1.1"?>
    <iCalendar xmlns="urn:ietf:params:xml:ns:xcal">
      <vcalendar>
        <version>2.0</version>
        <prodid>-//hacksw/handcal//NONSGML v1.0//EN</prodid>
        <vevent>
          <dtstart>19970714T170000Z</dtstart>
          <dtend>19970715T035959Z</dtend>
            <summary xml:lang="en_US">Bastille Day Party</summary>
        </vevent>
        </vcalendar>
    </iCalendar>

### [The xupl](/nicerobot/xupl/tree/master/language/src/test/resources/output/ical.xup) ###

    ?xml "1.1" version.
    iCalendar "urn:ietf:params:xml:ns:xcal" xmlns{
      vcalendar{
        version: "2.0"
        prodid: "-//hacksw/handcal//NONSGML v1.0//EN"
        vevent{
          dtstart: "19970714T170000Z"
          dtend: "19970715T035959Z"
          summary "en_US" lang/xml: "Bastille Day Party"
        }
      }
    }

---

### Apache Ivy

    <ivy-module version="2.0">
      <info organisation="org"
           module="standalone"
           revision="working"
       />
      <dependencies>
        <dependency org="apache" name="commons-lang" rev="2.0" conf="default->*"/>
      </dependencies>
    </ivy-module>

### As xupl

    ivy-module version 2.0 {
      info "org" organisation, "standalone" module, "working" revision
      dependencies {
        dependency "apache" org, "commons-lang" name, rev 2.0, "default->*" conf
      }
    }

---
 
### For this XML ###

    <xsl:stylesheet version="2.0"
      xmlns:xhtml="http://www.w3.org/1999/xhtml"
      xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
      <xsl:template match="*">
        [<xsl:value-of select="." />]
      </xsl:template>
    </xsl:stylesheet>

### The xupl ###

    stylesheet/xsl 2.0 version,
      "http://www.w3.org/1999/xhtml" xhtml@xmlns,
      "http://www.w3.org/1999/XSL/Transform" xsl@xmlns
    {
      template/xsl match "*": "[" value-of/xsl select ".". "]"
    }

## XML Specifics ##

## CDATA ##

### For this XML ###

    <![CDATA[<greeting>Hello, world!</greeting>]]> 

### The xupl ###

    '["&lt;greeting>Hello, world!&lt;/greeting>"]

## Comments ##

### For this XML ###

    <!-- declarations for <head> & <body> -->

### The xupl ###

Multi-line comment

    /* declarations for <head> & <body> */

Single-line comment

    // declarations for <head> & <body>

## Processing Instructions ##

### For this XML ###

    <?xml version="1.1"?>
    <greeting>Hello, world!</greeting>

### The xupl ###

    ?xml "1.1" version.
    greeting: "Hello, world!"

## Document Type Declaration (or a catchall) ##

Anything not directly supported above can be included verbatim with #|"""..."""

### For this XML ###

    <!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN"
     "http://www.apple.com/DTDs/PropertyList-1.0.dtd">

### The xupl ###

    !!!plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN"
    "http://www.apple.com/DTDs/PropertyList-1.0.dtd"!!!
    
### For this XML ###

    <?xml version="1.1" encoding="UTF-8" ?>
    <!DOCTYPE greeting [
    <!ELEMENT greeting (#PCDATA)>
    ]>
    <greeting>Hello, world!</greeting>

### The xupl ###

    ?xml with "1.1" version, and "UTF-8" encoding
    !!!greeting [
    <!ELEMENT greeting (#PCDATA)>
    ]>!!!
    greeting: "Hello, world!"
