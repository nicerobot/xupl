v0.1+13137.1223

# a-z.xupl

input:

    a b c d e f g h i j k l m n o p q r s t u v w x y z {
      a . b. c .d . e.f. g .h. i .j.k .l.m. n . . . o . o o o . p.q.r.s.t.u.v.w.x.y.z.
      a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,
      ! # $ % ^ * ( ) - _ + ~ ` [ ] \ : ; < > / ?
    }


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <a b="c" d="e" f="g" h="i" j="k" l="m" n="o" p="q" r="s" t="u" v="w" x="y">
      <. b.="c" class="d" class="" e.f.="g" class="h." i=".j.k" class="l.m." n="." class="" class="" o="." o="o" o="." p.q.r.s.t.u.v.w.x.y.z.="a" b="c" d="e" f="g" h="i" j="k" l="m" n="o" p="q" r="s" t="u" v="w" x="y" z="!" id="" $="%" at="" *="(" )="-" _="+" duration="" `="[" ]="\" type="" ;="&lt;" >="/"/>
    </a>

# activity.xupl

input:

    activity {
    
      if "Some Test", use "true" {
        to "activity 1";
    
        if {
          to "activity 3";
    
          if "last test" {
            to "activity 7";
          } else {
            to "activity 8";
          }
          return;
        }
    
        if "Other test" {
          to "activity 5", left;
        } else {
          to "activity 6";
        }
    
      } else use "false" {
        to "activity 2";
      }
    
    }


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <activity>
      <if use="Some Test" if="last test">activity 1;activity 3;<to/><to/></if>
      <if>activity 5</if>
      <to/>
    </activity>
    <use>activity 2;</use>

# activity2.xupl

input:

    activity {
    
      title "Servlet Container"
    
      start "ClickServlet.handleRequest ()"
      to "new Page"
      
      if "Page.onSecurityCheck", use "true" {
        to "Page.onInit ()"
    
        if not "isForward ?" {
          to "Process controls"
          
          if not "continue processing ?" {
            to REDIRECT_CHECK
          }
          /* implied else */
        }
        /* implied else */
        /* implied synchronization */
    
        if "is Post?" {
          to "Page.onPost ()"
        } else {
          to "Page.onGet ()"
        }
        to "Page.onRender ()"
    
      }
      /* implied else */
      /* implied synchronization */
    
      REDIRECT_CHECK {
        if "Do redirect ?" {
          to "redirect request"
        } else {
          if "Do Forward?" {
            to "Forward request", left
          } else {
            to "Render page template", right
          }
        }
      }
    
      to "Page.onDestroy ()"
    
    }


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <activity>
      <title start="Servlet Container" to="ClickServlet.handleRequest ()" if="new Page" use="Page.onSecurityCheck" if="not" if="not">Page.onInit ()Process controls<REDIRECT_CHECK/></* implied="else"/><implied else="*/" href="/*" implied="synchronization" */="if">Page.onPost ()</implied><else><to/>Page.onRender ()</else></* implied="else" */="/*" implied="synchronization" */="REDIRECT_CHECK"><if>redirect request</if><else><if>Forward request</if><to right="Render page template"/></else><//*><to/></title>
    </activity>

# activity3.xupl

input:

    activity {
      start parallel {
        to "Parallel Activity 1"
        to "Parallel Activity 2"
      }
      to end
    }


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <activity>
      <start to="Parallel Activity 2">Parallel Activity 1</start>
      <to/>
    </activity>

# attr1.xupl

input:

    note to "Tove", from "Jani" {
      subject {"Reminder"}
      body {"Don't forget me this weekend!"}


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <note to="Tove" from="Jani">
      <subject>Reminder</subject>
      <body>Dont forget me this weekend!</body>
    </note>

# build.xupl

input:

    /*
       Licensed to the Apache Software Foundation (ASF) under one
       or more contributor license agreements.  See the NOTICE file
       distributed with this work for additional information
       regarding copyright ownership.  The ASF licenses this file
       to you under the Apache License, Version 2.0 (the
       "License"); you may not use this file except in compliance
       with the License.  You may obtain a copy of the License at
    
         http://www.apache.org/licenses/LICENSE-2.0
    
       Unless required by applicable law or agreed to in writing,
       software distributed under the License is distributed on an
       "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
       KIND, either express or implied.  See the License for the
       specific language governing permissions and limitations
       under the License.    
    */
    project "go-ivy" name, "go" default, "antlib:org.apache.ivy.ant" ivy@xmlns:
      property "ivy.install.version" name, "2.0.0-beta1" value
      property "ivy.jar.dir" name, "${basedir}/ivy" value
      property "ivy.jar.file" name, "${ivy.jar.dir}/ivy.jar" value
      property "build.dir" name, "build" value
      property "src.dir" name, "src" value
      target "download-ivy" name, "skip.download" unless:
        mkdir "${ivy.jar.dir}" dir
        echo "installing ivy..." message
        get "http://repo1.maven.org/maven2/org/apache/ivy/ivy/${ivy.install.version}/ivy-${ivy.install.version}.jar" src, "${ivy.jar.file}" dest, "true" usetimestamp.
      target "install-ivy" name, "download-ivy" depends, "--> install ivy" description:
        path "ivy.lib.path" id:
          fileset "${ivy.jar.dir}" dir, "*.jar" includes.
        taskdef "org/apache/ivy/ant/antlib.xml" resource, "antlib:org.apache.ivy.ant" uri, "ivy.lib.path" classpathref.
      target "go" name, "install-ivy, generate-src" depends, "--> resolve dependencies, compile and run the project" description:
        echo "using ivy to resolve commons-lang 2.1..." message
        cachepath@ivy "commons-lang" organisation, "commons-lang" module, "2.1" revision, "lib.path.id" pathid, "true" inline
        echo "compiling..." message
        mkdir "${build.dir}" dir
        javac "${src.dir}" srcdir, "${build.dir}" destdir, "lib.path.id" classpathref
        echo: "
    We are now ready to execute our simple program with its dependency on commons-lang
    Let's go!
    		"
        java "example.Hello" classname:
          classpath:
            path "lib.path.id" refid
            path "${build.dir}" location...
      target "generate-src" name:
        mkdir "${src.dir}/example" dir
        echo "${src.dir}/example/Hello.java" file: 
          """
    package example;
    
    import org.apache.commons.lang.WordUtils;
    
    public class Hello {
        public static void main(String[] args) {
            String  message = "hello ivy !";
            System.out.println("standard message : " + message);
            System.out.println("capitalized by " + WordUtils.class.getName() 
                                    + " : " + WordUtils.capitalizeFully(message));
        }
    }
            """.
      target "clean" name, "--> clean the project" description:
        delete "true" includeemptydirs, "true" quiet:
          fileset "${src.dir}" dir
          fileset "${build.dir}" dir..
      target "clean-ivy" name, "--> clean the ivy installation" description:
        delete "${ivy.jar.dir}" dir.
      target "clean-cache" name, "install-ivy" depends, "--> clean the ivy cache" description:
        cleancache@ivy..


output:


# bump.xupl

input:

    bump=
      name "bump"
      version v1.0.0
      description "bump project"
      main "index"
    {
      repository=
        type "git",
        url http://github.com/nicerobot/text-plain.git
      
      devDependencies should v0.3.1
      dependencies {
        express v2.5.4
        jade from v0.0.1
        connect-mongo from v0.0.1
        async from v0.0.1
        mongoose from v2.0.0
        mongoose-auth from v0.0.11
        less from v0.0.1
      }
      engines {
        node from v0.3.0, to v0.7.0
      }
      contributors {
        name "Name1", twitter @twitter, email mailto:email, url http://about.me
        name "Name2", twitter @twitter, email mailto:email, url http://about.me
        name "Name3", twitter @twitter, email mailto:email, url http://about.me
        name "Name4", twitter @twitter, email mailto:email, url http://about.me
        name "Name5", twitter @twitter, email mailto:email, url http://about.me
      }
    }


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <bump= name="bump" version="v1.0.0" description="bump project" main="index">
      <repository= type="git" url="http://github.com/nicerobot/text-plain.git" devDependencies="should" v0.3.1="dependencies">
        <express v2.5.4="jade" from="v0.0.1" connect-mongo="from" v0.0.1="async" from="v0.0.1" mongoose="from" v2.0.0="mongoose-auth" from="v0.0.11" less="from"/>
        <node from="v0.3.0" to="v0.7.0"/>
        <contributors>
          <name twitter="Name1" project="twitter" email="mailto:email" url="http://about.me" name="Name2" twitter="@twitter" email="mailto:email" url="http://about.me" name="Name3" twitter="@twitter" email="mailto:email" url="http://about.me" name="Name4" twitter="@twitter" email="mailto:email" url="http://about.me" name="Name5" twitter="@twitter" email="mailto:email" url="http://about.me"/>
        </contributors>
      </repository=>
    </bump=>

# calendar.xupl

input:

    ?xml version "1.0", encoding "UTF-8"
    control@metal "calendar" name,
      "http://www.xmtl.org/version/2010/04" metal@xmlns,
      "http://www.w3.org/1999/xhtml" xmlns {
    
      div "previous-month${index}", "navigator previous-month" class: " ← "
      div "month" class: "${this.now.getMonth() + 1}/${this.now.getYear() + 1900}"
      div "next-month${index}", "navigator net-month" class: " → "
      
      control@metal "week" name {
        control@metal "days" name {
          div "day${calendar.index}-${week.index}-${index}", "week${week.index} day${index} day" class: "${this.day}"
        }
      }
      
      init@metal "[]" this, "months" variable {
        """
        function getDate(t) {
          if (t instanceof Date) {
            return t;
          }
            
          if ("string" === typeof(t)) {
            try {
              t = new Date(t);
            } 
            catch (e) {
              t = undefined;
            }
          }    
          if (!(t instanceof Date) || !!!+t) {
            t = new Date();
          }
          return t;
        }
        for (var a=0; a<months.length; a++) {
          var t = getDate(months[a]);
          var y = t.getYear() + 1900;
          var m = t.getMonth();
          var firstWeekday = new Date(y,m,1).getDay();
          var lastDay = new Date(y,m+1,1,0,0,-1).getDate();
          this.push({
            "now"  : t,
            "week" : [0, 1, 2, 3, 4, 5, 6].map(function(w,wi){ return { 
              "days" : ["Su", "M", "Tu", "W", "Th", "F", "Sa"].map(function(d,di) {
                 var day = +w * 7 - (6 + firstWeekday) + di;
                 if (0 == wi) day = d;
                 else if (1 > day || day > lastDay) day = "";
                 return { "day": String(day) };
              })
            };}),
          });
        }
      """
      }
    }


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <version encoding="1.0" control@metal="UTF-8" name="calendar" metal@xmlns="http://www.xmtl.org/version/2010/04" xmlns="http://www.w3.org/1999/xhtml">
      <div id="previous-month${index}" id="navigator previous-month" class:=" ← " div="month" class:="${this.now.getMonth() + 1}/${this.now.getYear() + 1900}" div="next-month${index}" class:="navigator net-month" control@metal=" → " name="week">
        <control@metal name="days">
          <div id="day${calendar.index}-${week.index}-${index}" id="week${week.index} day${index} day" class:="${this.day}"/>
        </control@metal>
        <init@metal this="[]" variable="months">"
        function getDate(t) {
          if (t instanceof Date) {
            return t;
          }
            
          if (string" === typeof(t)) {
            try {
              t = new Date(t);
            } 
            catch (e) {
              t = undefined;
            }
          }    
          if (!(t instanceof Date) || !!!+t) {
            t = new Date();
          }
          return t;
        }
        for (var a=0; a&lt;months.length; a++) {
          var t = getDate(months[a]);
          var y = t.getYear() + 1900;
          var m = t.getMonth();
          var firstWeekday = new Date(y,m,1).getDay();
          var lastDay = new Date(y,m+1,1,0,0,-1).getDate();
          this.push({
            now"  : t,
            week" : [0, 1, 2, 3, 4, 5, 6].map(function(w,wi){ return { 
              days" : [Su", M", Tu", W", Th", F", Sa"].map(function(d,di) {
                 var day = +w * 7 - (6 + firstWeekday) + di;
                 if (0 == wi) day = d;
                 else if (1 &gt; day || day &gt; lastDay) day = ";
                 return { day": String(day) };
              })
            };}),
          });
        }
      "</init@metal>
      </div>
    </version>

# cdata.xupl

input:

    cdata-test {
    [["
    
    this'll be verbatim
    
    "]]
    
      xmldecl {
        one XML
        execute {[["addPI("xml");"]]}
        one attributes
        one '.'
        execute {[["closeNode();"]]}
      }
    }


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <cdata-test>
      <[["
    
    thisll be verbatim
    
    ]]>
        <XML/>
        <one attributes="one" execute=".">
          <[["closeNode();]]/>
        </one>
      </[["
    
    thisll be verbatim
    
    ]]>
    </cdata-test>

# components.xupl

input:

    component {
    
      package "Some Group" {
        interface "HTTP": "First Component"
        component "Another Component"
      }
      package "Other Groups" {
        interface "FTP": "Second Component"
        component "First Component":  "FTP"
      }
    
      note left, of "HTTP" : "Web Service only"
    
      note right, of "First Component":"""
      A note can also
      be on several lines
      """
    
      component "Component", left, right, up, down
    
    }


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <component><package component="First Component">HTTP:</package>Other Groups<interface id="FTP:" id="Second Component" component="First Component:"/><left of="HTTP" type="" note="Web Service only" right="of" component="First Component:&quot;&quot;&#10;  A note can also&#10;  be on several lines&#10;  &quot;" left="Component" right="up"/></component>

# css.xupl

input:

    // Converted from the LESS examples home page.
    style@text/css=
      the-border 1px 0x123 1.2 1.2e-4 0123 "Hello"
      base-color #111
      red        #842210
        #header=
          color        ${base-color * 3}
          border-left  ${the-border}
          border-right ${the-border * 2}
        #footer=
          color        ${base-color + #003300}
          border-color ${css.desaturate(red, 10%)}
        #header
          h1=
            font-size 26px
            font-weight bold
            border (1px solid black)
          p=
            font-size 12px
            ${#footer}
              a=
                text-decoration none
                  ${this}:hover=
                    border-width 1px
                    ${#header.h1}
    
    /* Produces:
    #header {
      color: #333;
      border-left: 1px;
      border-right: 2px;
    }
    #footer { 
      color: #114411;
      border-color: #7d2717;
    }
    #header h1 {
      font-size: 26px;
      font-weight: bold;
      border: 1px solid black;
    }
    #header p {
      font-size: 12px;
      color: #114411;
      border-color: #7d2717;
    }
    #header p a {
      text-decoration: none;
    }
    #header p a:hover {
      border-width: 1px;
      font-size: 26px;
      font-weight: bold;
      border: 1px solid black;
    }
    */


output:

    STATE ERROR: Expected ATT [0010], is [0008]
    <?xml version="1.1" encoding="UTF-8"?>
    <// Converted="from" the="LESS" examples="home" page.="style@text/css=" the-border="1px" 0x123="1.2" 1.2e-4="0123" base-color="Hello" id="111" red="#842210" id="header=" color="$">
      <base-color *="3"/>
      <border-left/>
      <border-right>
        <*/>
        <color>
          <+ id="003300"/>
          <border-color $="css.desaturate(red">
            <10%)/>
            <#header h1=="font-size" 26px="font-weight" bold="border" (1px="solid" black)="p=" font-size="12px"/>
            <a= text-decoration="none"/>
            <:hover= border-width="1px"/>
            </* Produces:="#header">
              <color: id="333;" border-left:="1px;" border-right:="2px;"/>
              <#footer>
                <color: id="114411;" border-color:="#7d2717;"/>
                <#header>
                  <26px; font-weight:="bold;" border:="1px" solid="black;"/>
                  <#header>
                    <12px; color:="#114411;" border-color:="#7d2717;"/>
                    <#header p="a">
                      <text-decoration:/>
                      <p>
                        <1px; font-size:="26px;" font-weight:="bold;" border:="1px" solid="black;"/>
                      </p>
                    </#header>
                  </#header>
                </#header>
              </#footer>
            <//*>
          </border-color>
        </color>
      </border-right>
    <///>

# cssb.xupl

input:

    # Converted (and expanded upon) from the LESS examples home page.
    // the attributes of the root css tag are variable declarations.
    // &{} represents calls to functions or paths (like XPath) to content in the document.
    // Each tag's body has access to ${this}. That's a little inconsistent wrt to &{} in general. TODO: fix
    // TODO: How to handle the colon "a:hover" to not be "a {hover}"?
    style@text/css the-border 1px, base-color #111, red #842210 {
      a|b.c.d|b. c. d|1.2 v1.2.3 123 #anna
      p#header=
        color        &{base-color * 3}
        border-left  &{the-border}
        border-right &{the-border * 2}
      #footer=
        color        &{base-color + #003300}
        border-color &{css.desaturate(red, 10%)}
      #header {
        h1 font-size 26px, font-weight bold, border (1px solid black)
        p font-size 12px, &{#footer}, background-image url('img_tree.png') {
          a text-decoration none {
            &{this}:hover border-width 1px, "&{#header.h1}"
          }
        }
      }
    }
    
    /* Produces:
    #header {
      color: #333;
      border-left: 1px;
      border-right: 2px;
    }
    #footer { 
      color: #114411;
      border-color: #7d2717;
    }
    #header h1 {
      font-size: 26px;
      font-weight: bold;
      border: 1px solid black;
      background-image url('img_tree.png')
    }
    #header p {
      font-size: 12px;
      color: #114411;
      border-color: #7d2717;
    }
    #header p a {
      text-decoration: none;
    }
    #header p a:hover {
      border-width: 1px;
      font-size: 26px;
      font-weight: bold;
      border: 1px solid black;
    }
    */


output:

    STATE ERROR: Expected ATT [0010], is [0008]
    STATE ERROR: Expected ATT [0010], is [0008]
    <?xml version="1.1" encoding="UTF-8"?>
    <# Converted="(and" expanded="upon)" from="the" LESS="examples" home="page." href="//" the="attributes" of="the" root="css" tag="are" variable="declarations." href="//"/>
    <calls to="functions" or="paths" (like="XPath)" to="content" in="the" document.="//" Each="tag's body has access to ${this}. Thats" a="little" inconsistent="wrt" to="&amp;"/>
    <in general.="TODO:" fix="//" TODO:="How" to="handle" the="colon" to="a:hover" not="be" style@text/css="a {hover}?" the-border="1px" base-color="#111" red="#842210">
      <a|b.c.d|b. c.="d|1.2" v1.2.3="123" id="anna" p#header=="color">
        <*/>
        <&>
          <the-border/>
          <border-right>
            <*/>
            <color>
              <+ id="003300"/>
              <border-color &="css.desaturate(red">
                <10%)/>
                <#header>
                  <h1 font-size="26px" font-weight="bold" border="(1px" solid="black)" p="font-size" 12px="&amp;">
                    <#footer/>
                    <background-image>
                      <text-decoration>
                        <this/>
                        <:hover border-width="1px"/>
                      </text-decoration>
                    </background-image>
                  </h1>
                  <Produces: id="header">
                    <color: id="333;" border-left:="1px;" border-right:="2px;"/>
                    <#footer>
                      <color: id="114411;" border-color:="#7d2717;"/>
                      <#header>
                        <26px; font-weight:="bold;" border:="1px" solid="black;" background-image="url('img_tree.png)"/>
                        <#header>
                          <12px; color:="#114411;" border-color:="#7d2717;"/>
                          <#header p="a">
                            <text-decoration:/>
                            <p>
                              <1px; font-size:="26px;" font-weight:="bold;" border:="1px" solid="black;"/>
                              <*//>
                            </p>
                          </#header>
                        </#header>
                      </#header>
                    </#footer>
                  </Produces:>
                </#header>
              </border-color>
            </color>
          </border-right>
        </&>
      </a|b.c.d|b.>
    </in>

# dns.xupl

input:

    /* http://bouncybouncy.net/ramblings/posts/thrift_and_protocol_buffers/ */
    package passive_dns {
      message DnsRecord {
        key required, string: 1;
        value required, string: 2;
        first required, string: 3;
        last required, string: 4;
        type optional, string, default "A": 5;
        ttl optional, int32, default 86400: 6;
      }
      message DnsResponse {
        records repeated, DnsRecord: 1;
      }
    }


output:

    <?xml version="1.1" encoding="UTF-8"?>
    </* http://bouncybouncy.net/ramblings/posts/thrift_and_protocol_buffers/="*/" package="passive_dns">
      <message>
        <required string:="1;" value="required" string:="2;" first="required" string:="3;" last="required" string:="4;" type="optional" string="default" 5;="A:" ttl="optional" int32="default" 86400:="6;"/>
        <message>
          <repeated DnsRecord:="1;"/>
        </message>
      </message>
    <//*>

# doctype.xupl

input:

    doctype {
    !!!
    whatever
    !!!
    }


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <doctype>
      <!!! whatever="!!!"/>
    </doctype>

# eof1.xupl

input:

    html:body


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <html:body/>

# eof2.xupl

input:

    html:body:"xupl stream test"


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <html:body:"xupl stream test/>

# eof3.xupl

input:

    html:body#top
      "xupl stream test"
      a#go "/nicerobot/text-plain"


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <html:body#top a#go="xupl stream test"/>

# eof4.xupl

input:

    html:body:
    "xupl stream test"
    a "/nicerobot/text-plain":
    "text-plain"


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <html:body: a="xupl stream test" id="/nicerobot/text-plain:" id="text-plain"/>

# eof5.xupl

input:

    html:head{title{"eof test"}}body
      "xupl stream test"
      a "/nicerobot/text-plain":"text-plain"


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <html:head>
      <title>eof test</title>
    </html:head>
    <body a="xupl stream test"/>

# eof6.xupl

input:

    html:head{title{"eof test"}}body#top
      "xupl stream test"
      a "/nicerobot/text-plain":"text-plain"
      div#content
        span:"streams"
        img "logo.png"


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <html:head>
      <title>eof test</title>
    </html:head>
    <body#top a="xupl stream test" div#content="/nicerobot/text-plain:&quot;text-plain" span:"streams="img"/>

# html.xupl

input:

    html lang en
      head
        title: "&{pageTitle}"
        script@text/javascript
          if (foo) {
             bar()
          }
      body
        h1: "Xupl - template engine"
        div#container
          if@tup isXupl
            p@html: "You are amazing"
          else@tup
            p@html Get on it!


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <html lang="en" head="title:" script@text/javascript="&amp;{pageTitle}" if="(foo)">
      <bar()/>
      <body h1:="Xupl - template engine" div#container="if@tup" isXupl="p@html:" else@tup="You are amazing" p@html="Get"/>
    </html>

# htmlcssjs.xupl

input:

    html lang en
      head
        title: "&{pageTitle}"
        style@text/css=
          no-open@tup
          #header
            h1=
              font-size 26px
              font-weight bold
            p=
              font-size 12px
              &{#footer} {
                a=
                  text-decoration none
                  &{this}:hover=
                    border-width 1px
                    &{#header.h1}
              }
        
        script@text/javascript
          if (foo) {
             bar()
          }
    
      body
        h1: "Xupl - template engine"
        div#header
          if@tup isXupl
            p@html: "Xupl is amazing"
          else@tup
            p@html Get on it!


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <script@text/javascript if="(foo)">
      <bar()/>
      <body h1:="Xupl - template engine" div#header="if@tup" isXupl="p@html:" else@tup="Xupl is amazing" p@html="Get"/>
    </script@text/javascript>
    <a= text-decoration="none"/>
    <:hover= border-width="1px"/>

# htmlcssjsb.xupl

input:

    html lang en
      head
        title|"&{pageTitle}"
        style@text/css
          #header
            h1 font-size 26px, font-weight bold
            p font-size 12px, &{#footer}
              a text-decoration none
                &{this}:hover border-width 1px, &{#header.h1}
    
        script@text/javascript
          if (foo) {
             bar()
          }
    
      body
        h1|"Xupl - template engine"
        div#header
          if@tup isXupl:p@html| "Xupl is amazing"
          else@tup| p@html "Get on it!"


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <html lang="en" head="title|&quot;&amp;{pageTitle}" style@text/css="#header" h1="font-size" 26px="font-weight" bold="p" font-size="12px"/>
    <a text-decoration="none"/>
    <:hover border-width="1px"/>
    <script@text/javascript if="(foo)">
      <bar()/>
      <body h1|"Xupl - template engine="div#header" if@tup="isXupl:p@html|" else@tup|="Xupl is amazing" p@html="Get on it!"/>
    </script@text/javascript>

# ivy.xupl

input:

    ivy-module v2.0 version
      info organization "org.rnix",
        module "xupl",
        revision v1.0,
        status "integration",
        publication ${date '%Y%m%d%H%M%S'}
      publications
        artifact "xupl" name, "tbz2" ext, "tbz2" type


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <ivy-module v2.0="version" info="organization" module="org.rnix" revision="xupl" v1.0="status" publication="integration">%Y%m%d%H%M%S</ivy-module>
    <publications artifact="xupl" name="tbz2" ext="tbz2"/>

# json.xupl

input:

    {
      popularity 3
      translated true
      adult false
      language "en"
      original_name "The Princess Bride"
      name "The Princess Bride"
      alternative_name "The Princess Bride (Special Edition 1987)"
      movie_type "movie"
      id 2493
      imdb_id "tt0093779"
      url http://www.themoviedb.org/movie/2493
      votes 40
      rating 9.0
      status "Released"
      tagline "She gets kidnapped. He gets killed. But it all ends up okay."
      certification "PG"
      overview """In this enchantingly cracked fairy tale the beautiful Princess Buttercup and the dashing Westley must overcome staggering odds to find happiness amid six-fingered swordsmen murderous princes Sicilians and rodents of unusual size. But even death can't stop these true lovebirds from triumphing."""
      keywords
       {
           "grandfather grandson relationship"
           "fairy tale"
           "illness and injury"
           "grandpa"
           "water monster"
       }
      released "1987-01-27"
      runtime 98
      budget 16000000
      revenue 30857814
      homepage ""
      trailer http://www.youtube.com/watch?v=VYgcrny2hRs
      genres {
           {
              type "genre"
              url http://themoviedb.org/genre/action
              name "Action"
              id 28
           }
           {
              type "genre"
              url http://themoviedb.org/genre/adventure
              name "Adventure"
              id 12
           }
           {
              type "genre"
              url http://themoviedb.org/genre/comedy
              name "Comedy"
              id 35
           }
           {
              type "genre"
              url http://themoviedb.org/genre/family
              name "Family"
              id 10751
           }
           {
              type "genre"
              url http://themoviedb.org/genre/fantasy
              name "Fantasy"
              id 14
           }
           {
              type "genre"
              url http://themoviedb.org/genre/romance
              name "Romance"
              id 10749
           }
       }
      studios 
       {
           {
              url http://www.themoviedb.org/company/1577
              name "Act III Communications"
              id 1577
           }
           {
              url http://www.themoviedb.org/company/1578
              name "Buttercup Films"
              id 1578
           }
       }
      languages_spoken 
       {
           {
              code "en"
              name "English"
              native_name ""
           }
       }
      countries 
       {
           {
              code "US"
              name "United States of America"
              url http://www.themoviedb.org/country/us
           }
       }
      posters 
       {
           {
              image 
               {
                  type "poster"
                  size "thumb"
                  height 138
                  width 92
                  url http://cf2.imgobject.com/t/p/w92/cMpDzgkAsLUWEjPGzglD2coI4TY.jpg
                  id "4ea601caa34d4b3d2d00111d"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w154"
                  height 231
                  width 154
                  url http://cf2.imgobject.com/t/p/w154/cMpDzgkAsLUWEjPGzglD2coI4TY.jpg
                  id "4ea601caa34d4b3d2d00111d"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "cover"
                  height 278
                  width 185
                  url http://cf2.imgobject.com/t/p/w185/cMpDzgkAsLUWEjPGzglD2coI4TY.jpg
                  id "4ea601caa34d4b3d2d00111d"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w342"
                  height 513
                  width 342
                  url http://cf2.imgobject.com/t/p/w342/cMpDzgkAsLUWEjPGzglD2coI4TY.jpg
                  id "4ea601caa34d4b3d2d00111d"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "mid"
                  height 750
                  width 500
                  url http://cf2.imgobject.com/t/p/w500/cMpDzgkAsLUWEjPGzglD2coI4TY.jpg
                  id "4ea601caa34d4b3d2d00111d"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "original"
                  height 1500
                  width 1000
                  url http://cf2.imgobject.com/t/p/original/cMpDzgkAsLUWEjPGzglD2coI4TY.jpg
                  id "4ea601caa34d4b3d2d00111d"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "thumb"
                  height 138
                  width 92
                  url http://cf2.imgobject.com/t/p/w92/sIz90HKdw1vKOlRknDy0CwnwV8.jpg
                  id "4ea601bfa34d4b3d2d001117"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w154"
                  height 231
                  width 154
                  url http://cf2.imgobject.com/t/p/w154/sIz90HKdw1vKOlRknDy0CwnwV8.jpg
                  id "4ea601bfa34d4b3d2d001117"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "cover"
                  height 278
                  width 185
                  url http://cf2.imgobject.com/t/p/w185/sIz90HKdw1vKOlRknDy0CwnwV8.jpg
                  id "4ea601bfa34d4b3d2d001117"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w342"
                  height 513
                  width 342
                  url http://cf2.imgobject.com/t/p/w342/sIz90HKdw1vKOlRknDy0CwnwV8.jpg
                  id "4ea601bfa34d4b3d2d001117"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "mid"
                  height 750
                  width 500
                  url http://cf2.imgobject.com/t/p/w500/sIz90HKdw1vKOlRknDy0CwnwV8.jpg
                  id "4ea601bfa34d4b3d2d001117"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "original"
                  height 1500
                  width 1000
                  url http://cf2.imgobject.com/t/p/original/sIz90HKdw1vKOlRknDy0CwnwV8.jpg
                  id "4ea601bfa34d4b3d2d001117"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "thumb"
                  height 138
                  width 92
                  url http://cf2.imgobject.com/t/p/w92/elDCZNBSZFHncpYThVdNM73c0P.jpg
                  id "4ea601e2a34d4b3d2d00112b"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w154"
                  height 231
                  width 154
                  url http://cf2.imgobject.com/t/p/w154/elDCZNBSZFHncpYThVdNM73c0P.jpg
                  id "4ea601e2a34d4b3d2d00112b"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "cover"
                  height 278
                  width 185
                  url http://cf2.imgobject.com/t/p/w185/elDCZNBSZFHncpYThVdNM73c0P.jpg
                  id "4ea601e2a34d4b3d2d00112b"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w342"
                  height 513
                  width 342
                  url http://cf2.imgobject.com/t/p/w342/elDCZNBSZFHncpYThVdNM73c0P.jpg
                  id "4ea601e2a34d4b3d2d00112b"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "mid"
                  height 750
                  width 500
                  url http://cf2.imgobject.com/t/p/w500/elDCZNBSZFHncpYThVdNM73c0P.jpg
                  id "4ea601e2a34d4b3d2d00112b"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "original"
                  height 1500
                  width 1000
                  url http://cf2.imgobject.com/t/p/original/elDCZNBSZFHncpYThVdNM73c0P.jpg
                  id "4ea601e2a34d4b3d2d00112b"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "thumb"
                  height 136
                  width 92
                  url http://cf2.imgobject.com/t/p/w92/5jq9727Tdd6I7uwIuifuvreOuhP.jpg
                  id "4ea601b9a34d4b3d2d001113"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w154"
                  height 228
                  width 154
                  url http://cf2.imgobject.com/t/p/w154/5jq9727Tdd6I7uwIuifuvreOuhP.jpg
                  id "4ea601b9a34d4b3d2d001113"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "cover"
                  height 274
                  width 185
                  url http://cf2.imgobject.com/t/p/w185/5jq9727Tdd6I7uwIuifuvreOuhP.jpg
                  id "4ea601b9a34d4b3d2d001113"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w342"
                  height 506
                  width 342
                  url http://cf2.imgobject.com/t/p/w342/5jq9727Tdd6I7uwIuifuvreOuhP.jpg
                  id "4ea601b9a34d4b3d2d001113"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "mid"
                  height 740
                  width 500
                  url http://cf2.imgobject.com/t/p/w500/5jq9727Tdd6I7uwIuifuvreOuhP.jpg
                  id "4ea601b9a34d4b3d2d001113"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "original"
                  height 755
                  width 510
                  url http://cf2.imgobject.com/t/p/original/5jq9727Tdd6I7uwIuifuvreOuhP.jpg
                  id "4ea601b9a34d4b3d2d001113"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "thumb"
                  height 136
                  width 92
                  url http://cf2.imgobject.com/t/p/w92/tgzrbVNNzA3LUbGrc6Xjef0NnYs.jpg
                  id "4ea601bca34d4b3d2d001115"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w154"
                  height 228
                  width 154
                  url http://cf2.imgobject.com/t/p/w154/tgzrbVNNzA3LUbGrc6Xjef0NnYs.jpg
                  id "4ea601bca34d4b3d2d001115"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "cover"
                  height 274
                  width 185
                  url http://cf2.imgobject.com/t/p/w185/tgzrbVNNzA3LUbGrc6Xjef0NnYs.jpg
                  id "4ea601bca34d4b3d2d001115"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w342"
                  height 507
                  width 342
                  url http://cf2.imgobject.com/t/p/w342/tgzrbVNNzA3LUbGrc6Xjef0NnYs.jpg
                  id "4ea601bca34d4b3d2d001115"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "mid"
                  height 741
                  width 500
                  url http://cf2.imgobject.com/t/p/w500/tgzrbVNNzA3LUbGrc6Xjef0NnYs.jpg
                  id "4ea601bca34d4b3d2d001115"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "original"
                  height 1000
                  width 675
                  url http://cf2.imgobject.com/t/p/original/tgzrbVNNzA3LUbGrc6Xjef0NnYs.jpg
                  id "4ea601bca34d4b3d2d001115"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "thumb"
                  height 138
                  width 92
                  url http://cf2.imgobject.com/t/p/w92/91vbhwKVOwlLSgIC8H9jVmcHJhy.jpg
                  id "4ea601d4a34d4b3d2d001123"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w154"
                  height 231
                  width 154
                  url http://cf2.imgobject.com/t/p/w154/91vbhwKVOwlLSgIC8H9jVmcHJhy.jpg
                  id "4ea601d4a34d4b3d2d001123"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "cover"
                  height 278
                  width 185
                  url http://cf2.imgobject.com/t/p/w185/91vbhwKVOwlLSgIC8H9jVmcHJhy.jpg
                  id "4ea601d4a34d4b3d2d001123"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w342"
                  height 513
                  width 342
                  url http://cf2.imgobject.com/t/p/w342/91vbhwKVOwlLSgIC8H9jVmcHJhy.jpg
                  id "4ea601d4a34d4b3d2d001123"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "mid"
                  height 750
                  width 500
                  url http://cf2.imgobject.com/t/p/w500/91vbhwKVOwlLSgIC8H9jVmcHJhy.jpg
                  id "4ea601d4a34d4b3d2d001123"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "original"
                  height 1500
                  width 1000
                  url http://cf2.imgobject.com/t/p/original/91vbhwKVOwlLSgIC8H9jVmcHJhy.jpg
                  id "4ea601d4a34d4b3d2d001123"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "thumb"
                  height 138
                  width 92
                  url http://cf2.imgobject.com/t/p/w92/xDkivPGSdLs62pLG0axX4cUqhSj.jpg
                  id "4ea601d1a34d4b3d2d001121"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w154"
                  height 231
                  width 154
                  url http://cf2.imgobject.com/t/p/w154/xDkivPGSdLs62pLG0axX4cUqhSj.jpg
                  id "4ea601d1a34d4b3d2d001121"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "cover"
                  height 278
                  width 185
                  url http://cf2.imgobject.com/t/p/w185/xDkivPGSdLs62pLG0axX4cUqhSj.jpg
                  id "4ea601d1a34d4b3d2d001121"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w342"
                  height 513
                  width 342
                  url http://cf2.imgobject.com/t/p/w342/xDkivPGSdLs62pLG0axX4cUqhSj.jpg
                  id "4ea601d1a34d4b3d2d001121"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "mid"
                  height 750
                  width 500
                  url http://cf2.imgobject.com/t/p/w500/xDkivPGSdLs62pLG0axX4cUqhSj.jpg
                  id "4ea601d1a34d4b3d2d001121"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "original"
                  height 1200
                  width 800
                  url http://cf2.imgobject.com/t/p/original/xDkivPGSdLs62pLG0axX4cUqhSj.jpg
                  id "4ea601d1a34d4b3d2d001121"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "thumb"
                  height 138
                  width 92
                  url http://cf2.imgobject.com/t/p/w92/mOm1ReZyLsBZ8GooHp7Lo55x2cD.jpg
                  id "4ea601c7a34d4b3d2d00111b"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w154"
                  height 231
                  width 154
                  url http://cf2.imgobject.com/t/p/w154/mOm1ReZyLsBZ8GooHp7Lo55x2cD.jpg
                  id "4ea601c7a34d4b3d2d00111b"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "cover"
                  height 278
                  width 185
                  url http://cf2.imgobject.com/t/p/w185/mOm1ReZyLsBZ8GooHp7Lo55x2cD.jpg
                  id "4ea601c7a34d4b3d2d00111b"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w342"
                  height 513
                  width 342
                  url http://cf2.imgobject.com/t/p/w342/mOm1ReZyLsBZ8GooHp7Lo55x2cD.jpg
                  id "4ea601c7a34d4b3d2d00111b"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "mid"
                  height 750
                  width 500
                  url http://cf2.imgobject.com/t/p/w500/mOm1ReZyLsBZ8GooHp7Lo55x2cD.jpg
                  id "4ea601c7a34d4b3d2d00111b"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "original"
                  height 1500
                  width 1000
                  url http://cf2.imgobject.com/t/p/original/mOm1ReZyLsBZ8GooHp7Lo55x2cD.jpg
                  id "4ea601c7a34d4b3d2d00111b"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "thumb"
                  height 136
                  width 92
                  url http://cf2.imgobject.com/t/p/w92/zmu9zexBIBUYMETjzR7Etuli0kv.jpg
                  id "4ea601cea34d4b3d2d00111f"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w154"
                  height 228
                  width 154
                  url http://cf2.imgobject.com/t/p/w154/zmu9zexBIBUYMETjzR7Etuli0kv.jpg
                  id "4ea601cea34d4b3d2d00111f"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "cover"
                  height 274
                  width 185
                  url http://cf2.imgobject.com/t/p/w185/zmu9zexBIBUYMETjzR7Etuli0kv.jpg
                  id "4ea601cea34d4b3d2d00111f"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w342"
                  height 506
                  width 342
                  url http://cf2.imgobject.com/t/p/w342/zmu9zexBIBUYMETjzR7Etuli0kv.jpg
                  id "4ea601cea34d4b3d2d00111f"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "mid"
                  height 740
                  width 500
                  url http://cf2.imgobject.com/t/p/w500/zmu9zexBIBUYMETjzR7Etuli0kv.jpg
                  id "4ea601cea34d4b3d2d00111f"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "original"
                  height 1500
                  width 1013
                  url http://cf2.imgobject.com/t/p/original/zmu9zexBIBUYMETjzR7Etuli0kv.jpg
                  id "4ea601cea34d4b3d2d00111f"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "thumb"
                  height 123
                  width 92
                  url http://cf2.imgobject.com/t/p/w92/5EE6e3XQfrjPAzYtXQJR3CKDzUU.png
                  id "4ea601c2a34d4b3d2d001119"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w154"
                  height 205
                  width 154
                  url http://cf2.imgobject.com/t/p/w154/5EE6e3XQfrjPAzYtXQJR3CKDzUU.png
                  id "4ea601c2a34d4b3d2d001119"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "cover"
                  height 246
                  width 185
                  url http://cf2.imgobject.com/t/p/w185/5EE6e3XQfrjPAzYtXQJR3CKDzUU.png
                  id "4ea601c2a34d4b3d2d001119"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w342"
                  height 455
                  width 342
                  url http://cf2.imgobject.com/t/p/w342/5EE6e3XQfrjPAzYtXQJR3CKDzUU.png
                  id "4ea601c2a34d4b3d2d001119"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "mid"
                  height 666
                  width 500
                  url http://cf2.imgobject.com/t/p/w500/5EE6e3XQfrjPAzYtXQJR3CKDzUU.png
                  id "4ea601c2a34d4b3d2d001119"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "original"
                  height 759
                  width 570
                  url http://cf2.imgobject.com/t/p/original/5EE6e3XQfrjPAzYtXQJR3CKDzUU.png
                  id "4ea601c2a34d4b3d2d001119"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "thumb"
                  height 108
                  width 92
                  url http://cf2.imgobject.com/t/p/w92/pO6ZyiPaFrtNJ1Q2zoHhHYnfOb9.jpg
                  id "4ea601dea34d4b3d2d001129"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w154"
                  height 181
                  width 154
                  url http://cf2.imgobject.com/t/p/w154/pO6ZyiPaFrtNJ1Q2zoHhHYnfOb9.jpg
                  id "4ea601dea34d4b3d2d001129"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "cover"
                  height 218
                  width 185
                  url http://cf2.imgobject.com/t/p/w185/pO6ZyiPaFrtNJ1Q2zoHhHYnfOb9.jpg
                  id "4ea601dea34d4b3d2d001129"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w342"
                  height 402
                  width 342
                  url http://cf2.imgobject.com/t/p/w342/pO6ZyiPaFrtNJ1Q2zoHhHYnfOb9.jpg
                  id "4ea601dea34d4b3d2d001129"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "mid"
                  height 588
                  width 500
                  url http://cf2.imgobject.com/t/p/w500/pO6ZyiPaFrtNJ1Q2zoHhHYnfOb9.jpg
                  id "4ea601dea34d4b3d2d001129"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "original"
                  height 957
                  width 814
                  url http://cf2.imgobject.com/t/p/original/pO6ZyiPaFrtNJ1Q2zoHhHYnfOb9.jpg
                  id "4ea601dea34d4b3d2d001129"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "thumb"
                  height 138
                  width 92
                  url http://cf2.imgobject.com/t/p/w92/2Z1lpxPoM39BVnsQjsBayuN5SvV.jpg
                  id "4ea601eaa34d4b3d2d00112f"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w154"
                  height 231
                  width 154
                  url http://cf2.imgobject.com/t/p/w154/2Z1lpxPoM39BVnsQjsBayuN5SvV.jpg
                  id "4ea601eaa34d4b3d2d00112f"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "cover"
                  height 278
                  width 185
                  url http://cf2.imgobject.com/t/p/w185/2Z1lpxPoM39BVnsQjsBayuN5SvV.jpg
                  id "4ea601eaa34d4b3d2d00112f"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w342"
                  height 513
                  width 342
                  url http://cf2.imgobject.com/t/p/w342/2Z1lpxPoM39BVnsQjsBayuN5SvV.jpg
                  id "4ea601eaa34d4b3d2d00112f"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "mid"
                  height 750
                  width 500
                  url http://cf2.imgobject.com/t/p/w500/2Z1lpxPoM39BVnsQjsBayuN5SvV.jpg
                  id "4ea601eaa34d4b3d2d00112f"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "original"
                  height 1500
                  width 1000
                  url http://cf2.imgobject.com/t/p/original/2Z1lpxPoM39BVnsQjsBayuN5SvV.jpg
                  id "4ea601eaa34d4b3d2d00112f"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "thumb"
                  height 138
                  width 92
                  url http://cf2.imgobject.com/t/p/w92/gpxjoE0yvRwIhFEJgNArtKtaN7S.jpg
                  id "4ea601eca34d4b3d2d001131"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w154"
                  height 231
                  width 154
                  url http://cf2.imgobject.com/t/p/w154/gpxjoE0yvRwIhFEJgNArtKtaN7S.jpg
                  id "4ea601eca34d4b3d2d001131"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "cover"
                  height 278
                  width 185
                  url http://cf2.imgobject.com/t/p/w185/gpxjoE0yvRwIhFEJgNArtKtaN7S.jpg
                  id "4ea601eca34d4b3d2d001131"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w342"
                  height 513
                  width 342
                  url http://cf2.imgobject.com/t/p/w342/gpxjoE0yvRwIhFEJgNArtKtaN7S.jpg
                  id "4ea601eca34d4b3d2d001131"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "mid"
                  height 750
                  width 500
                  url http://cf2.imgobject.com/t/p/w500/gpxjoE0yvRwIhFEJgNArtKtaN7S.jpg
                  id "4ea601eca34d4b3d2d001131"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "original"
                  height 1500
                  width 1000
                  url http://cf2.imgobject.com/t/p/original/gpxjoE0yvRwIhFEJgNArtKtaN7S.jpg
                  id "4ea601eca34d4b3d2d001131"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "thumb"
                  height 138
                  width 92
                  url http://cf2.imgobject.com/t/p/w92/peDFGjp7fYjCGBnrATpBC8D9mJT.jpg
                  id "4ea601f0a34d4b3d2d001133"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w154"
                  height 231
                  width 154
                  url http://cf2.imgobject.com/t/p/w154/peDFGjp7fYjCGBnrATpBC8D9mJT.jpg
                  id "4ea601f0a34d4b3d2d001133"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "cover"
                  height 278
                  width 185
                  url http://cf2.imgobject.com/t/p/w185/peDFGjp7fYjCGBnrATpBC8D9mJT.jpg
                  id "4ea601f0a34d4b3d2d001133"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w342"
                  height 513
                  width 342
                  url http://cf2.imgobject.com/t/p/w342/peDFGjp7fYjCGBnrATpBC8D9mJT.jpg
                  id "4ea601f0a34d4b3d2d001133"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "mid"
                  height 750
                  width 500
                  url http://cf2.imgobject.com/t/p/w500/peDFGjp7fYjCGBnrATpBC8D9mJT.jpg
                  id "4ea601f0a34d4b3d2d001133"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "original"
                  height 1500
                  width 1000
                  url http://cf2.imgobject.com/t/p/original/peDFGjp7fYjCGBnrATpBC8D9mJT.jpg
                  id "4ea601f0a34d4b3d2d001133"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "thumb"
                  height 107
                  width 92
                  url http://cf2.imgobject.com/t/p/w92/6ylRHxIVVHBC8fl2j5022XPxLg1.jpg
                  id "4ea601f3a34d4b3d2d001135"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w154"
                  height 180
                  width 154
                  url http://cf2.imgobject.com/t/p/w154/6ylRHxIVVHBC8fl2j5022XPxLg1.jpg
                  id "4ea601f3a34d4b3d2d001135"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "cover"
                  height 216
                  width 185
                  url http://cf2.imgobject.com/t/p/w185/6ylRHxIVVHBC8fl2j5022XPxLg1.jpg
                  id "4ea601f3a34d4b3d2d001135"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w342"
                  height 399
                  width 342
                  url http://cf2.imgobject.com/t/p/w342/6ylRHxIVVHBC8fl2j5022XPxLg1.jpg
                  id "4ea601f3a34d4b3d2d001135"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "mid"
                  height 583
                  width 500
                  url http://cf2.imgobject.com/t/p/w500/6ylRHxIVVHBC8fl2j5022XPxLg1.jpg
                  id "4ea601f3a34d4b3d2d001135"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "original"
                  height 1166
                  width 1000
                  url http://cf2.imgobject.com/t/p/original/6ylRHxIVVHBC8fl2j5022XPxLg1.jpg
                  id "4ea601f3a34d4b3d2d001135"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "thumb"
                  height 138
                  width 92
                  url http://cf2.imgobject.com/t/p/w92/oQBBe4NMj2NnJaFnpKE77u604Uc.jpg
                  id "4ea601f6a34d4b3d2d001137"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w154"
                  height 231
                  width 154
                  url http://cf2.imgobject.com/t/p/w154/oQBBe4NMj2NnJaFnpKE77u604Uc.jpg
                  id "4ea601f6a34d4b3d2d001137"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "cover"
                  height 278
                  width 185
                  url http://cf2.imgobject.com/t/p/w185/oQBBe4NMj2NnJaFnpKE77u604Uc.jpg
                  id "4ea601f6a34d4b3d2d001137"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w342"
                  height 513
                  width 342
                  url http://cf2.imgobject.com/t/p/w342/oQBBe4NMj2NnJaFnpKE77u604Uc.jpg
                  id "4ea601f6a34d4b3d2d001137"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "mid"
                  height 750
                  width 500
                  url http://cf2.imgobject.com/t/p/w500/oQBBe4NMj2NnJaFnpKE77u604Uc.jpg
                  id "4ea601f6a34d4b3d2d001137"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "original"
                  height 1500
                  width 1000
                  url http://cf2.imgobject.com/t/p/original/oQBBe4NMj2NnJaFnpKE77u604Uc.jpg
                  id "4ea601f6a34d4b3d2d001137"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "thumb"
                  height 138
                  width 92
                  url http://cf2.imgobject.com/t/p/w92/xQ3bhyatJKtSqaTKCKZkZzy7osi.jpg
                  id "4ea601faa34d4b3d2d001139"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w154"
                  height 231
                  width 154
                  url http://cf2.imgobject.com/t/p/w154/xQ3bhyatJKtSqaTKCKZkZzy7osi.jpg
                  id "4ea601faa34d4b3d2d001139"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "cover"
                  height 278
                  width 185
                  url http://cf2.imgobject.com/t/p/w185/xQ3bhyatJKtSqaTKCKZkZzy7osi.jpg
                  id "4ea601faa34d4b3d2d001139"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w342"
                  height 513
                  width 342
                  url http://cf2.imgobject.com/t/p/w342/xQ3bhyatJKtSqaTKCKZkZzy7osi.jpg
                  id "4ea601faa34d4b3d2d001139"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "mid"
                  height 750
                  width 500
                  url http://cf2.imgobject.com/t/p/w500/xQ3bhyatJKtSqaTKCKZkZzy7osi.jpg
                  id "4ea601faa34d4b3d2d001139"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "original"
                  height 1500
                  width 1000
                  url http://cf2.imgobject.com/t/p/original/xQ3bhyatJKtSqaTKCKZkZzy7osi.jpg
                  id "4ea601faa34d4b3d2d001139"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "thumb"
                  height 138
                  width 92
                  url http://cf2.imgobject.com/t/p/w92/9uhlG7kLKLCjPcvtPHVJD6871Ie.jpg
                  id "4ea601fda34d4b3d2d00113b"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w154"
                  height 231
                  width 154
                  url http://cf2.imgobject.com/t/p/w154/9uhlG7kLKLCjPcvtPHVJD6871Ie.jpg
                  id "4ea601fda34d4b3d2d00113b"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "cover"
                  height 278
                  width 185
                  url http://cf2.imgobject.com/t/p/w185/9uhlG7kLKLCjPcvtPHVJD6871Ie.jpg
                  id "4ea601fda34d4b3d2d00113b"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "w342"
                  height 513
                  width 342
                  url http://cf2.imgobject.com/t/p/w342/9uhlG7kLKLCjPcvtPHVJD6871Ie.jpg
                  id "4ea601fda34d4b3d2d00113b"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "mid"
                  height 750
                  width 500
                  url http://cf2.imgobject.com/t/p/w500/9uhlG7kLKLCjPcvtPHVJD6871Ie.jpg
                  id "4ea601fda34d4b3d2d00113b"
               }
           }
           {
              image 
               {
                  type "poster"
                  size "original"
                  height 1500
                  width 1000
                  url http://cf2.imgobject.com/t/p/original/9uhlG7kLKLCjPcvtPHVJD6871Ie.jpg
                  id "4ea601fda34d4b3d2d00113b"
               }
           }
       }
      backdrops 
       {
           {
              image 
               {
                  type "backdrop"
                  size "thumb"
                  height 169
                  width 300
                  url http://cf2.imgobject.com/t/p/w300/npIKgqpD8jbowpem9L3U4NUkEO3.jpg
                  id "4ea601b0a34d4b3d2d00110d"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "poster"
                  height 439
                  width 780
                  url http://cf2.imgobject.com/t/p/w780/npIKgqpD8jbowpem9L3U4NUkEO3.jpg
                  id "4ea601b0a34d4b3d2d00110d"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "w1280"
                  height 720
                  width 1280
                  url http://cf2.imgobject.com/t/p/w1280/npIKgqpD8jbowpem9L3U4NUkEO3.jpg
                  id "4ea601b0a34d4b3d2d00110d"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "original"
                  height 1080
                  width 1920
                  url http://cf2.imgobject.com/t/p/original/npIKgqpD8jbowpem9L3U4NUkEO3.jpg
                  id "4ea601b0a34d4b3d2d00110d"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "thumb"
                  height 169
                  width 300
                  url http://cf2.imgobject.com/t/p/w300/r4NIpC8i2707gq4zabVwIjDBgXA.jpg
                  id "4ea601d8a34d4b3d2d001125"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "poster"
                  height 439
                  width 780
                  url http://cf2.imgobject.com/t/p/w780/r4NIpC8i2707gq4zabVwIjDBgXA.jpg
                  id "4ea601d8a34d4b3d2d001125"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "w1280"
                  height 720
                  width 1280
                  url http://cf2.imgobject.com/t/p/w1280/r4NIpC8i2707gq4zabVwIjDBgXA.jpg
                  id "4ea601d8a34d4b3d2d001125"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "original"
                  height 1080
                  width 1920
                  url http://cf2.imgobject.com/t/p/original/r4NIpC8i2707gq4zabVwIjDBgXA.jpg
                  id "4ea601d8a34d4b3d2d001125"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "thumb"
                  height 169
                  width 300
                  url http://cf2.imgobject.com/t/p/w300/84FSFLeRHIR8MlyybcsXNbBnONY.jpg
                  id "4ea601e6a34d4b3d2d00112d"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "poster"
                  height 439
                  width 780
                  url http://cf2.imgobject.com/t/p/w780/84FSFLeRHIR8MlyybcsXNbBnONY.jpg
                  id "4ea601e6a34d4b3d2d00112d"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "w1280"
                  height 720
                  width 1280
                  url http://cf2.imgobject.com/t/p/w1280/84FSFLeRHIR8MlyybcsXNbBnONY.jpg
                  id "4ea601e6a34d4b3d2d00112d"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "original"
                  height 1080
                  width 1920
                  url http://cf2.imgobject.com/t/p/original/84FSFLeRHIR8MlyybcsXNbBnONY.jpg
                  id "4ea601e6a34d4b3d2d00112d"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "thumb"
                  height 169
                  width 300
                  url http://cf2.imgobject.com/t/p/w300/oaXyPP3HNW0gBl0ZGXLhJOdACeH.jpg
                  id "4ea601a3a34d4b3d2d001105"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "poster"
                  height 439
                  width 780
                  url http://cf2.imgobject.com/t/p/w780/oaXyPP3HNW0gBl0ZGXLhJOdACeH.jpg
                  id "4ea601a3a34d4b3d2d001105"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "w1280"
                  height 720
                  width 1280
                  url http://cf2.imgobject.com/t/p/w1280/oaXyPP3HNW0gBl0ZGXLhJOdACeH.jpg
                  id "4ea601a3a34d4b3d2d001105"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "original"
                  height 1080
                  width 1920
                  url http://cf2.imgobject.com/t/p/original/oaXyPP3HNW0gBl0ZGXLhJOdACeH.jpg
                  id "4ea601a3a34d4b3d2d001105"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "thumb"
                  height 169
                  width 300
                  url http://cf2.imgobject.com/t/p/w300/9HaAZUc3PdwBkPj5ZIakw2TYBfb.jpg
                  id "4ea601b3a34d4b3d2d00110f"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "poster"
                  height 439
                  width 780
                  url http://cf2.imgobject.com/t/p/w780/9HaAZUc3PdwBkPj5ZIakw2TYBfb.jpg
                  id "4ea601b3a34d4b3d2d00110f"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "w1280"
                  height 720
                  width 1280
                  url http://cf2.imgobject.com/t/p/w1280/9HaAZUc3PdwBkPj5ZIakw2TYBfb.jpg
                  id "4ea601b3a34d4b3d2d00110f"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "original"
                  height 1080
                  width 1920
                  url http://cf2.imgobject.com/t/p/original/9HaAZUc3PdwBkPj5ZIakw2TYBfb.jpg
                  id "4ea601b3a34d4b3d2d00110f"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "thumb"
                  height 169
                  width 300
                  url http://cf2.imgobject.com/t/p/w300/i4GOXWW1fBCUoWZBIhsPGDOcyRv.jpg
                  id "4ea601dba34d4b3d2d001127"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "poster"
                  height 439
                  width 780
                  url http://cf2.imgobject.com/t/p/w780/i4GOXWW1fBCUoWZBIhsPGDOcyRv.jpg
                  id "4ea601dba34d4b3d2d001127"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "w1280"
                  height 720
                  width 1280
                  url http://cf2.imgobject.com/t/p/w1280/i4GOXWW1fBCUoWZBIhsPGDOcyRv.jpg
                  id "4ea601dba34d4b3d2d001127"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "original"
                  height 1080
                  width 1920
                  url http://cf2.imgobject.com/t/p/original/i4GOXWW1fBCUoWZBIhsPGDOcyRv.jpg
                  id "4ea601dba34d4b3d2d001127"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "thumb"
                  height 169
                  width 300
                  url http://cf2.imgobject.com/t/p/w300/oCx1rh13ahSI3M5bAWMIcuJnMV8.jpg
                  id "4ea601b6a34d4b3d2d001111"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "poster"
                  height 439
                  width 780
                  url http://cf2.imgobject.com/t/p/w780/oCx1rh13ahSI3M5bAWMIcuJnMV8.jpg
                  id "4ea601b6a34d4b3d2d001111"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "w1280"
                  height 720
                  width 1280
                  url http://cf2.imgobject.com/t/p/w1280/oCx1rh13ahSI3M5bAWMIcuJnMV8.jpg
                  id "4ea601b6a34d4b3d2d001111"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "original"
                  height 720
                  width 1280
                  url http://cf2.imgobject.com/t/p/original/oCx1rh13ahSI3M5bAWMIcuJnMV8.jpg
                  id "4ea601b6a34d4b3d2d001111"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "thumb"
                  height 169
                  width 300
                  url http://cf2.imgobject.com/t/p/w300/o2gEauGHuMbWST0dDfgQWsxpx7Q.jpg
                  id "4ea601a7a34d4b3d2d001107"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "poster"
                  height 439
                  width 780
                  url http://cf2.imgobject.com/t/p/w780/o2gEauGHuMbWST0dDfgQWsxpx7Q.jpg
                  id "4ea601a7a34d4b3d2d001107"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "w1280"
                  height 720
                  width 1280
                  url http://cf2.imgobject.com/t/p/w1280/o2gEauGHuMbWST0dDfgQWsxpx7Q.jpg
                  id "4ea601a7a34d4b3d2d001107"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "original"
                  height 1080
                  width 1920
                  url http://cf2.imgobject.com/t/p/original/o2gEauGHuMbWST0dDfgQWsxpx7Q.jpg
                  id "4ea601a7a34d4b3d2d001107"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "thumb"
                  height 169
                  width 300
                  url http://cf2.imgobject.com/t/p/w300/jMzC0n5vHBuH9e9KLWQPOYKrITp.jpg
                  id "4ea601a9a34d4b3d2d001109"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "poster"
                  height 439
                  width 780
                  url http://cf2.imgobject.com/t/p/w780/jMzC0n5vHBuH9e9KLWQPOYKrITp.jpg
                  id "4ea601a9a34d4b3d2d001109"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "w1280"
                  height 720
                  width 1280
                  url http://cf2.imgobject.com/t/p/w1280/jMzC0n5vHBuH9e9KLWQPOYKrITp.jpg
                  id "4ea601a9a34d4b3d2d001109"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "original"
                  height 1080
                  width 1920
                  url http://cf2.imgobject.com/t/p/original/jMzC0n5vHBuH9e9KLWQPOYKrITp.jpg
                  id "4ea601a9a34d4b3d2d001109"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "thumb"
                  height 169
                  width 300
                  url http://cf2.imgobject.com/t/p/w300/aZ1dvyToBSf36l9My2UlFdYmghp.jpg
                  id "4ea601aca34d4b3d2d00110b"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "poster"
                  height 439
                  width 780
                  url http://cf2.imgobject.com/t/p/w780/aZ1dvyToBSf36l9My2UlFdYmghp.jpg
                  id "4ea601aca34d4b3d2d00110b"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "w1280"
                  height 720
                  width 1280
                  url http://cf2.imgobject.com/t/p/w1280/aZ1dvyToBSf36l9My2UlFdYmghp.jpg
                  id "4ea601aca34d4b3d2d00110b"
               }
           }
           {
              image 
               {
                  type "backdrop"
                  size "original"
                  height 1080
                  width 1920
                  url http://cf2.imgobject.com/t/p/original/aZ1dvyToBSf36l9My2UlFdYmghp.jpg
                  id "4ea601aca34d4b3d2d00110b"
               }
           }
       }
      cast 
       {
           {
              name "Rob Reiner"
              job "Director"
              department "Directing"
              character ""
              id 3026
              order 0
              cast_id 1
              url http://www.themoviedb.org/person/3026
              profile http://cf2.imgobject.com/t/p/w45/2zw9Iq9uo4vZiTQNQWdNFwbA1TA.jpg
           }
           {
              name "William Goldman"
              job "Screenplay"
              department "Writing"
              character ""
              id 348
              order 0
              cast_id 14
              url http://www.themoviedb.org/person/348
              profile http://cf2.imgobject.com/t/p/w45/7eBlS4cj4WAGzI38ef3EXAIKmOg.jpg
           }
           {
              name "William Goldman"
              job "Novel"
              department "Writing"
              character ""
              id 348
              order 0
              cast_id 15
              url http://www.themoviedb.org/person/348
              profile http://cf2.imgobject.com/t/p/w45/7eBlS4cj4WAGzI38ef3EXAIKmOg.jpg
           }
           {
              name "Peter Falk"
              job "Actor"
              department "Actors"
              character "Grandfather"
              id 2314
              order 0
              cast_id 11
              url http://www.themoviedb.org/person/2314
              profile http://cf2.imgobject.com/t/p/w45/4Nqjyzdeuvv6KANtQrSHwDD9bjd.jpg
           }
           {
              name "Cary Elwes"
              job "Actor"
              department "Actors"
              character "Westley"
              id 2130
              order 1
              cast_id 2
              url http://www.themoviedb.org/person/2130
              profile http://cf2.imgobject.com/t/p/w45/xRGHksJNOP94LXW0fffMmbw5yON.jpg
           }
           {
              name "Robin Wright"
              job "Actor"
              department "Actors"
              character "Buttercup"
              id 32
              order 2
              cast_id 3
              url http://www.themoviedb.org/person/32
              profile http://cf2.imgobject.com/t/p/w45/vfzQ4GJ8R7lxbtumayhUrhrhkLK.jpg
           }
           {
              name "Chris Sarandon"
              job "Actor"
              department "Actors"
              character "Prince Humperdinck"
              id 14541
              order 3
              cast_id 4
              url http://www.themoviedb.org/person/14541
              profile http://cf2.imgobject.com/t/p/w45/fnPawv8xOWpyKtSDmghz6cPJCEP.jpg
           }
           {
              name "Wallace Shawn"
              job "Actor"
              department "Actors"
              character "Vizzini"
              id 12900
              order 4
              cast_id 5
              url http://www.themoviedb.org/person/12900
              profile http://cf2.imgobject.com/t/p/w45/oDtP8BUpvclLwyYD8TbZ5YOmdjR.jpg
           }
           {
              name "Mandy Patinkin"
              job "Actor"
              department "Actors"
              character "Inigo Montoya"
              id 25503
              order 5
              cast_id 6
              url http://www.themoviedb.org/person/25503
              profile http://cf2.imgobject.com/t/p/w45/aiHwNtaKkUFd9kZ81wGhMANMKWC.jpg
           }
           {
              name "Andr\u00e9 the Giant"
              job "Actor"
              department "Actors"
              character "Fezzik"
              id 25504
              order 6
              cast_id 7
              url http://www.themoviedb.org/person/25504
              profile http://cf2.imgobject.com/t/p/w45/wVlQm9XEs9Nvvmf9vcXEFmXYae2.jpg
           }
           {
              name "Billy Crystal"
              job "Actor"
              department "Actors"
              character "Miracle Max"
              id 7904
              order 7
              cast_id 8
              url http://www.themoviedb.org/person/7904
              profile http://cf2.imgobject.com/t/p/w45/22PBSROMN6GmD7uxHpTNb0Unftt.jpg
           }
           {
              name "Carol Kane"
              job "Actor"
              department "Actors"
              character "Valerie"
              id 10556
              order 8
              cast_id 9
              url http://www.themoviedb.org/person/10556
              profile http://cf2.imgobject.com/t/p/w45/1SF1dOdG2jzQaiT8OUtiY9z6DrN.jpg
           }
           {
              name "Fred Savage"
              job "Actor"
              department "Actors"
              character "Grandson"
              id 2223
              order 9
              cast_id 10
              url http://www.themoviedb.org/person/2223
              profile http://cf2.imgobject.com/t/p/w45/q6u7yhINrALOC9wD2tc6mifQ9sJ.jpg
           }
           {
              name "Christopher Guest"
              job "Actor"
              department "Actors"
              character "Count Tyrone Rugen"
              id 13524
              order 10
              cast_id 12
              url http://www.themoviedb.org/person/13524
              profile http://cf2.imgobject.com/t/p/w45/fhEQq0q2aR2sh4HU824xunoyAce.jpg
           }
           {
              name "Mel Smith"
              job "Actor"
              department "Actors"
              character "The Albino"
              id 141450
              order 11
              cast_id 17
              url http://www.themoviedb.org/person/141450
              profile http://cf2.imgobject.com/t/p/w45/t9aojcL97IStdae9fP90D84NlSJ.jpg
           }
       }
      version 1897
      last_modified_at "2011-11-03 04:14:59 UTC"
    }


output:


# meta1.xupl

input:

    html{head{title{"XUPL"}}body{
    	div .xuplclass #xuplid [somedata {
    		a //github.com/nicerobot/text-plain/wiki {"text-plain.org"}
    		importance !hot ^adate {
    			geo =25.72N80.28W :style {}
    		  subject @xuplproj {"Reminder"}


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <html>
      <head>
        <title>XUPL</title>
      </head>
      <body>
        <div class="xuplclass" id="xuplid" data="somedata">
          <a href="//github.com/nicerobot/text-plain/wiki">text-plain.org</a>
          <importance priority="hot" at="adate">
            <geo location="25.72N80.28W" type="style"/>
            <subject project="xuplproj">Reminder</subject>
          </importance>
        </div>
      </body>
    </html>

# note.xupl

input:

    note {
      to {"Tove"}
      from {"Jani"}
      subject {"Reminder"}
      body {"Don't forget me this weekend!"}


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <note>
      <to>Tove</to>
      <from>Jani</from>
      <subject>Reminder</subject>
      <body>Dont forget me this weekend!</body>
    </note>

# pom.xupl

input:

    project http://maven.apache.org/POM/4.0.0 , http://maven.apache.org/maven-v4_0_0.xsd schemaLocation@xsi=
      modelVersion v4.0.0
      groupId org.nicerobot.ex
      artifactId java-ex
      version v0.0.1-SNAPSHOT
        dependencies
          dependency=
            groupId junit
            artifactId junit
            version v4.7
            type jar
            scope test
          dependency=
            groupId org.nicerobot
            artifactId java-util
            version v0.0.1-SNAPSHOT
          dependency=
            groupId org.nicerobot
            artifactId java-io
            version v0.0.1-SNAPSHOT
          dependency=
            groupId javax.activation
            artifactId activation
            version v1.1.1
            type jar
            scope compile
          dependency=
            groupId javax.servlet
            artifactId servlet-api
            version v2.5
            type jar
            scope compile
        build
          plugins
            plugin=
              groupId org.apache.maven.plugins
              artifactId maven-war-plugin
              version v2.0.2
                configuration
                  webappDirectory war...


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <project http://maven.apache.org/POM/4.0.0="http://maven.apache.org/maven-v4_0_0.xsd" schemaLocation@xsi=="modelVersion" v4.0.0="groupId" org.nicerobot.ex="artifactId" java-ex="version" v0.0.1-SNAPSHOT="dependencies" dependency=="groupId" junit="artifactId" junit="version" v4.7="type" jar="scope" test="dependency=" groupId="org.nicerobot" artifactId="java-util" version="v0.0.1-SNAPSHOT" dependency=="groupId" org.nicerobot="artifactId" java-io="version" v0.0.1-SNAPSHOT="dependency=" groupId="javax.activation" artifactId="activation" version="v1.1.1" type="jar" scope="compile" dependency=="groupId" javax.servlet="artifactId" servlet-api="version" v2.5="type" jar="scope" compile="build" plugins="plugin=" groupId="org.apache.maven.plugins" artifactId="maven-war-plugin" version="v2.0.2" configuration="webappDirectory"/>

# q3.xupl

input:

    overview """In triumphing."""


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <overview/>

# sequence.xupl

input:

    sequence {
    
      actors {
        user "User"
        participant A "First Class"
        participant B "Second Class"
        participant C "Last Class"
      }
    
      messages {
        user "DoWork" A {
          A "<< createRequest >>" B {
            B "DoWork" C {
              return "WorkDone", destroy
            }
            return "Request <u>Created</u>"
          }
          return "Done"
        }
      }
    }


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <sequence>
      <actors>
        <user participant="User" A="First Class" participant="B" participant="Second Class" C="Last Class"/>
        <messages>
          <user A="DoWork">
            <A B="&lt;&lt; createRequest &gt;&gt;">
              <B C="DoWork"><return destroy="WorkDone"/><return/>Done</B>
            </A>
          </user>
        </messages>
      </actors>
    </sequence>

# sequence2.xupl

input:

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


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <sequence>
      <actors>
        <user participant="User" view=":View" participant="controller" participant=":Controller" model=":Model"/>
        <messages>
          <user view="request()" view="handleEvent()" controller="controller" model="queryInformation()" controller="doValidation()" controller="view" controller="updateView()" user="notifyUser()"/>
        </messages>
      </actors>
    </sequence>

# test-defaults.stuxt.xupl

input:

    default
      "xlst" xlst@xmlns,
      "html" html@xmlns,
      "metal" metal@xmlns
    {
      a@html http://my.href|"link text"
      a@xlst http://my.id|"link test"
    
      value-of@xlst "x/b"
      value-of@html "x/b"
    
      control@metal "test" {
        div "&{test.index}"|"metal"
      }
    
      control@html "test" {
        div@xslt "${test.index}"|"metal"
      }
    
    }


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <default xlst@xmlns="xlst" html@xmlns="html" metal@xmlns="metal">
      <a@html http://my.href|"link text="a@xlst" http://my.id|"link test="value-of@xlst" value-of@html="x/b" control@metal="x/b">&amp;{test.index}|"metal</a@html>
      <control@html>${test.index}|"metal</control@html>
    </default>

# topology.xupl

input:

    topology {
      host name "${hostname1}", CPU "${CPUGhz1}", Memory "${memory1}", OS "${OS1}", user "${username1}", path "${path1}" {
        software name "${software1}"
      }
      host name "${hostname2}", CPU "${CPUGhz2}", Memory "${memory2}", OS "${OS2}", user "${username2}", path "${path2}" {
        software name "${software2}"
      }
      host name "${hostname3}", CPU "${CPUGhz3}", Memory "${memory3}", OS "${OS3}", user "${username3}", path "${path3}" {
        software name "${software3}"
      }
      protocol "${protocol}" { host "${hostname1}". host "${hostname2}". }
    }


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <topology>
      <host name="${hostname1}" CPU="${CPUGhz1}" Memory="${memory1}" OS="${OS1}" user="${username1}" path="${path1}">
        <software name="${software1}"/>
        <host name="${hostname2}" CPU="${CPUGhz2}" Memory="${memory2}" OS="${OS2}" user="${username2}" path="${path2}">
          <software name="${software2}"/>
          <host name="${hostname3}" CPU="${CPUGhz3}" Memory="${memory3}" OS="${OS3}" user="${username3}" path="${path3}">
            <software name="${software3}"/>
            <protocol host="${hostname2}.">${hostname1}.</protocol>
          </host>
        </host>
      </host>
    </topology>

# xupl-config.xupl

input:

    xupl-config {
      default-attributes {
        namespace http://www.w3.org/1999/xhtml {
          default a "href"
        }
        namespace http://www.xuplml.org/xmlns/version/2010/05 {
        }
        namespace http://www.w3.org/1999/XSL/Transform {
          default value-of "select"
        }
        namespace http://www.xmtl.org/version/2010/04 {
          default control "name"
        }    
      }
    }


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <xupl-config>
      <default-attributes>
        <namespace>
          <a/>
          <http://www.xuplml.org/xmlns/version/2010/05/>
          <namespace>
            <value-of/>
            <http://www.xmtl.org/version/2010/04>
              <default control="name"/>
            </http://www.xmtl.org/version/2010/04>
          </namespace>
        </namespace>
      </default-attributes>
    </xupl-config>

# xupl-config1.xupl

input:

    xupl-config
      default-attributes
        namespace http://www.w3.org/1999/xhtml
          default a "href"
        namespace http://www.xuplml.org/xmlns/version/2010/05
        namespace http://www.w3.org/1999/XSL/Transform
          default value-of "select"
        namespace http://www.xmtl.org/version/2010/04
          default control "name"


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <xupl-config default-attributes="namespace" http://www.w3.org/1999/xhtml="default" a="href" namespace="http://www.xuplml.org/xmlns/version/2010/05" namespace="http://www.w3.org/1999/XSL/Transform" default="value-of" namespace="select" http://www.xmtl.org/version/2010/04="default" control="name"/>
