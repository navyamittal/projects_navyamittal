/**
 * passport module
 * @const passport
 */
const passport = require('passport');
const GoogleStrategy = require( 'passport-google-oauth2' ).Strategy;

const GOOGLE_CLIENT_ID = '723902555125-jni07n0st3knd88rje52b68u7a8s8004.apps.googleusercontent.com';
const GOOGLE_CLIENT_SECRET = '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';

/**
 * Google authentication
 * @function
 * @memberof module:passport
 * @name googlePassport
 */
passport.use(new GoogleStrategy({
    clientID:     GOOGLE_CLIENT_ID,
    clientSecret: GOOGLE_CLIENT_SECRET,
    // callbackURL: "http://localhost:3001/google/callback",
    callbackURL: "http://sharetea-08r.onrender.com/google/callback",
    passReqToCallback   : true
  },
  function(request, accessToken, refreshToken, profile, done) {
    // User.findOrCreate({ googleId: profile.id }, function (err, user) {
    //   return done(err, user);
    // });
    return done(null, profile);
  }
));

/**
 * Serialize user for authentication
 * @function
 * @memberof module:passport
 * @name serializeUser
 */
passport.serializeUser(function(user, done) {
    done(null, user);
});

/**
 * Deserialize user for authentication
 * @function
 * @memberof module:passport
 * @name deserializeUser
 */
passport.deserializeUser(function(user, done) {
    done(null, user);
});