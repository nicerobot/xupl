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
# html.xupl

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
