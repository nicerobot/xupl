[![Build Status](https://buildhive.cloudbees.com/job/nicerobot/job/xupl/badge/icon)](https://buildhive.cloudbees.com/job/nicerobot/job/xupl/)

# See the [`xupl`](https://github.com/nicerobot/xupl/wiki) wiki

Xupl enables the writing HTML like this

    html {
    	head {title{"Xupl"}}
    } body {
    	div #xuplid .xuplclass {
    		a //github.com/nicerobot/text-plain/wiki {"text-plain.org"}

and getting this:

    <html>
      <head>
        <title>Xupl</title>
      </head>
      <body>
        <div id="xuplid" class="xuplclass">
          <a href="//github.com/nicerobot/text-plain/wiki">text-plain.org</a>
        </div>
      </body>
    </html>

