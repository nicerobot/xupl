v0.1+13145.0847

# [attr1.xupl](attr1.xupl)

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



# [html.xupl](html.xupl)

input:

    html{head{title{"XUPL"}}body{p{"Supple"}p{"XUPL"}p{Awesome}


output:

    <?xml version="1.1" encoding="UTF-8"?>
    <html>
      <head>
        <title>XUPL</title>
      </head>
      <body>
        <p>Supple</p>
        <p>XUPL</p>
        <p>
          <Awesome/>
        </p>
      </body>
    </html>



# [meta1.xupl](meta1.xupl)

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



# [note.xupl](note.xupl)

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




## missing:

    missing expect/a-z.xml
    missing expect/activity.xml
    missing expect/activity2.xml
    missing expect/activity3.xml
    missing expect/build.xml
    missing expect/bump.xml
    missing expect/calendar.xml
    missing expect/cdata.xml
    missing expect/components.xml
    missing expect/css.xml
    missing expect/cssb.xml
    missing expect/dns.xml
    missing expect/doctype.xml
    missing expect/eof1.xml
    missing expect/eof2.xml
    missing expect/eof3.xml
    missing expect/eof4.xml
    missing expect/eof5.xml
    missing expect/eof6.xml
    missing expect/html1.xml
    missing expect/htmlcssjs.xml
    missing expect/htmlcssjsb.xml
    missing expect/ivy.xml
    missing expect/json.xml
    missing expect/pom.xml
    missing expect/q3.xml
    missing expect/sequence.xml
    missing expect/sequence2.xml
    missing expect/test-defaults.stuxt.xml
    missing expect/topology.xml
    missing expect/xupl-config.xml
    missing expect/xupl-config1.xml
