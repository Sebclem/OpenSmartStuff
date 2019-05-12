var express = require('express');
var router = express.Router();

module.exports.router = function (passport) {
    router.get('/signup', function (req, res, next) {
        if (req.isAuthenticated())
            res.redirect('/');
        res.render('signup',{error: req.flash('error')});
    });

    router.post('/signup', passport.authenticate('local-signup', {successRedirect: '/', failureRedirect: '/signup', failureFlash: true}));


    router.get('/signin', function (req, res, next) {
        if (req.isAuthenticated())
            res.redirect('/');
        res.render('signin', {error: req.flash('error')});
    });

    router.post('/signin', passport.authenticate('local-signin', {
        successRedirect: '/login-done',
        failureRedirect: '/signin',
        failureFlash: true
    }));

    router.get('/logout', function (req, res, next) {
        req.session.destroy(function (err) {
            res.redirect('/signin');
        });
    });

    router.get('/login-done', function (req, res, next) {
        if(req.cookies.destination){
            res.clearCookie('destination');
            res.redirect(req.cookies.destination);
        }
        else{
            res.redirect("/");
        }
    });



    return router;
};

module.exports.isLoggedIn = function (req, res, next) {
    if (req.isAuthenticated())
        return next();
    res.redirect('/signin');
};

module.exports.isLoggedInApi = function (req, res, next) {
    if (req.isAuthenticated())
        return next();
    res.status(401);
    res.send();
};

