var nodemailer = require('nodemailer');
const express = require('express');

const app = express();

var transporter = nodemailer.createTransport({
    service: 'gmail',
    auth: {
        user: 'iotmailesp@gmail.com',
        pass: 'rarm whbu hcir gtxp'
    }
});

app.get('/', (req, res) => {

    const message = req.query.message

    console.log(`message is => ${message}`)

    const mailOptions = {
        from: 'iotmailesp@gmail.com', // sender address
        to: 'yassinajbouni120@gmail.com', // list of receivers
        subject: 'test', // Subject line
        html: `<p>${message}</p>`// plain text body
    };

    transporter.sendMail(mailOptions, function (err, info) {
        if (err){
            console.log(err)
            res.status(500).send('<h1>error.</h1>')
        }
        else {
            console.log(info);
            res.status(200).send('<h1>Email sent!</h1>')
        }
    });
})

app.listen(80,()=>{
    console.log('app running.')
})
