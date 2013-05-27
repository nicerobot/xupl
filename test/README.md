v0.1+13147.1851

# [attr1.brace.xupl](attr1.brace.xupl)

input:

    note to "Tove", from "Jani" {
      subject {"Reminder"}
      body {"Don't forget me this weekend!"}


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <note>
      <to>Tove<from>Jani<subject>Reminder</subject><body>Don't forget me this weekend!</body></from></to>
    </note>


    2,3c2,4
    < <note>
    <   <to>Tove<from>Jani<subject>Reminder</subject><body>Don't forget me this weekend!</body></from></to>
    ---
    > <note to="Tove" from="Jani">
    >   <subject>Reminder</subject>
    >   <body>Dont forget me this weekend!</body>

# [dns.brace.xupl](dns.brace.xupl)

input:

    // http://bouncybouncy.net/ramblings/posts/thrift_and_protocol_buffers/="*/" package="passive_dns"
    package passive_dns {
    	message DnsRecord {
    		key required, string {1}
    		value required, string {2}
    		first required, string {3}
    		last required, string {4}
    		type optional, string, default "A" {5}
    		ttl optional, int32, default 86400 {6}
    	}
    	message DnsResponse {
    		records repeated, DnsRecord {1}
    	}
    }


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <//>
      <http://bouncybouncy.net/ramblings/posts/thrift_and_protocol_buffers/="*/>
        <package="passive_dns>
          <package>
            <passive_dns>
              <message>
                <DnsRecord>
                  <key>
                    <required>
                      <string>
                        <1/>
                        <value>
                          <required>
                            <string>
                              <2/>
                              <first>
                                <required>
                                  <string>
                                    <3/>
                                    <last>
                                      <required>
                                        <string>
                                          <4/>
                                          <type>
                                            <optional>
                                              <string>
                                                <default>A<5/><ttl><optional><int32><default><86400><6/></86400><message><DnsResponse><records><repeated><DnsRecord><1/></DnsRecord></repeated></records></DnsResponse></message></default></int32></optional></ttl></default>
                                              </string>
                                            </optional>
                                          </type>
                                        </string>
                                      </required>
                                    </last>
                                  </string>
                                </required>
                              </first>
                            </string>
                          </required>
                        </value>
                      </string>
                    </required>
                  </key>
                </DnsRecord>
              </message>
            </passive_dns>
          </package>
        </package="passive_dns>
      </http://bouncybouncy.net/ramblings/posts/thrift_and_protocol_buffers/="*/>
    <///>


    2,50c2,15
    < <//>
    <   <http://bouncybouncy.net/ramblings/posts/thrift_and_protocol_buffers/="*/>
    <     <package="passive_dns>
    <       <package>
    <         <passive_dns>
    <           <message>
    <             <DnsRecord>
    <               <key>
    <                 <required>
    <                   <string>
    <                     <1/>
    <                     <value>
    <                       <required>
    <                         <string>
    <                           <2/>
    <                           <first>
    <                             <required>
    <                               <string>
    <                                 <3/>
    <                                 <last>
    <                                   <required>
    <                                     <string>
    <                                       <4/>
    <                                       <type>
    <                                         <optional>
    <                                           <string>
    <                                             <default>A<5/><ttl><optional><int32><default><86400><6/></86400><message><DnsResponse><records><repeated><DnsRecord><1/></DnsRecord></repeated></records></DnsResponse></message></default></int32></optional></ttl></default>
    <                                           </string>
    <                                         </optional>
    <                                       </type>
    <                                     </string>
    <                                   </required>
    <                                 </last>
    <                               </string>
    <                             </required>
    <                           </first>
    <                         </string>
    <                       </required>
    <                     </value>
    <                   </string>
    <                 </required>
    <               </key>
    <             </DnsRecord>
    <           </message>
    <         </passive_dns>
    <       </package>
    <     </package="passive_dns>
    <   </http://bouncybouncy.net/ramblings/posts/thrift_and_protocol_buffers/="*/>
    < <///>
    ---
    > <!-- http://bouncybouncy.net/ramblings/posts/thrift_and_protocol_buffers/="*/" package="passive_dns" -->
    > <package passive_dns="True">
    >   <message DnsRecord="True">
    >     <key required="True" string="True">1</key>
    >     <value required="True" string="True">2</value>
    >     <first required="True" string="True">3</first>
    >     <last required="True" string="True">4</last>
    >     <type optional="True" string="True" default="True" value="A">5</type>
    >     <ttl optional="True" int32="True" default="True" value="86400">6</ttl>
    >   </message>
    >   <message DnsResponse="True">
    >     <records repeated="True" DnsRecord="True">1</records>
    >   </message>
    > </package>

# [html.brace.xupl](html.brace.xupl)

input:

    html{head{title{"XUPL"}}body{p #name{"Supple"}p #content{"XUPL"}p .footer{Awesome}


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <html>
      <head>
        <title>XUPL</title>
      </head>
      <body>
        <p id="name">Supple</p>
        <p id="content">XUPL</p>
        <p class="footer">
          <Awesome/>
        </p>
      </body>
    </html>



# [meta1.brace.xupl](meta1.brace.xupl)

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



# [note.brace.xupl](note.brace.xupl)

input:

    
    
    
      note {
      to {"Tove"}
      from {"Jani"}
      subject {"Reminder"}
      body {"Don't forget me this weekend!"


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <note>
      <to>Tove</to>
      <from>Jani</from>
      <subject>Reminder</subject>
      <body>Don't forget me this weekend!</body>
    </note>




## missing:

    missing expect/activity.brace.xml
    missing expect/activity2.xml
    missing expect/activity3.xml
    missing expect/build.xml
    missing expect/bump.xml
    missing expect/calendar.xml
    missing expect/cdata.xml
    missing expect/components.xml
    missing expect/css.xml
    missing expect/cssb.xml
    missing expect/dns.brace.xml
    missing expect/doctype.xml
    missing expect/eof1.xml
    missing expect/eof2.xml
    missing expect/htmlcssjsb.brace.xml
    missing expect/ivy.brace.xml
    missing expect/namespaces.brace.xml
    missing expect/pom.brace.xml
    missing expect/sequence.brace.xml
    missing expect/topology.brace.xml
    missing expect/xupl-config.brace.xml
    missing expect/xupl-config.indent.xml
