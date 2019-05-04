const createError = require('http-errors');
const express = require('express');
const path = require('path');
const cookieParser = require('cookie-parser');
const logger = require('morgan');
const flash = require('connect-flash');



const oauth = require('./routes/oauth');



const app = express();

const passport   = require('passport');
const session    = require('express-session');
const bodyParser = require('body-parser');
const models = require("./models");

//For BodyParser
app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

// For Passport
app.use(session({ secret: 'keyboard cat',resave: true, saveUninitialized:true})); // session secret
app.use(passport.initialize());
app.use(passport.session()); // persistent login sessions
require('./config/passport/passport.js')(passport, models.user);

//sync database
models.sequelize.sync({alter: true}).then(function () {
    console.log('[DB INIT] Nice! Database looks fine');
}).catch(function (err) {
    console.error(err, "[DB INIT] Something went wrong with the Database Update!")
});


const indexRouter = require('./routes/index');
const usersRouter = require('./routes/users');
const authManager = require('./routes/auth-manager').router(passport);
const google = require('./routes/GoogleSmartHome');


// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));
app.use(flash());

app.use('/', indexRouter);
app.use('/users', usersRouter);
app.use('/', oauth);
app.use('/', authManager);
app.post('/googleAPI', google);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
    next(createError(404));
});

// error handler
app.use(function(err, req, res, next) {
    // set locals, only providing error in development
    res.locals.message = err.message;
    res.locals.error = req.app.get('env') === 'development' ? err : {};

    // render the error page
    res.status(err.status || 500);
    res.render('error');
});

module.exports = app;
