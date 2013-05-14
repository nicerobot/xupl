# note.xupl

input:

    note {
      to {"Tove"}
      from {"Jani"}
      subject {"Reminder"}
      body {"Don't forget me this weekend!"}
    }

output:

    <?xml version="1.1" encoding="UTF-8"?>
    <note>
      <to>Tove</to>
      <from>Jani</from>
      <subject>Reminder</subject>
      <body>Dont forget me this weekend!</body>
    </note>

# html.xupl

input:

    html{head{title{"XUPL"}}body{"Supple"}}

output:

    <?xml version="1.1" encoding="UTF-8"?>
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
    }

output:

    <?xml version="1.1" encoding="UTF-8"?>
    <note to="Tove" from="Jani">
      <subject>Reminder</subject>
      <body>Dont forget me this weekend!</body>
    </note>
