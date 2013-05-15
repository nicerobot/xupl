# note.xupl

input:

    note {
      to {"Tove"}
      from {"Jani"}
      subject {"Reminder"}
      body {"Don't forget me this weekend!"}

output:

    <note>
      <to>Tove</to>
      <from>Jani</from>
      <subject>Reminder</subject>
      <body>Dont forget me this weekend!</body>
    </note>

# meta1.xupl

- Tests implemention of automatic attribute-names based on [metatext](//github.com/nicerobot/text-plain/wiki).

input:

    html{head{title{"XUPL"}}body{
      div .xuplclass #xuplid {
        a [//github.com/nicerobot/text-plain/wiki {"text-plain.org"}
        importance !hot ^adate {
          geo =25.72N80.28W :context {}
          subject @xuplproj {"Reminder"}

output:

    <html>
      <head>
        <title>XUPL</title>
      </head>
      <body>
        <div class="xuplclass" id="xuplid">
          <a href="//github.com/nicerobot/text-plain/wiki">text-plain.org</a>
          <importance priority="hot" at="adate">
            <geo location="25.72N80.28W" context="context"/>
            <subject project="xuplproj">Reminder</subject>
          </importance>
        </div>
      </body>
    </html>


# html.xupl

input:

    html{head{title{"XUPL"}}body{"Supple"}}

output:

    <html>
      <head>
        <title>XUPL</title>
      </head>
      <body>Supple</body>
    </html>

# attr1.xupl

input:

    note to "Tove", from "Jani" {
      subject {"Reminder"}
      body {"Don't forget me this weekend!"}

output:

    <note to="Tove" from="Jani">
      <subject>Reminder</subject>
      <body>Dont forget me this weekend!</body>
    </note>
