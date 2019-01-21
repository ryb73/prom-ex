open Reduice.Promise;

let map: ('a => 'b, t('a)) => t('b);
let thenResolve: ('a, t('b)) => t('a);
let tap: ('a => 'b, t('a)) => t('a);
let invertOptional: option(t('a)) => t(option('a));
let thenMaybe: ('a => t('b), t(option('a))) => t(option('b));
let mapMaybe: ('a => 'b, t(option('a))) => t(option('b));
let tapMaybe: ('a => 'b, t(option('a))) => t(option('a));
let unwrapResult: t(Belt.Result.t('a, 'b)) => t('a);
let amend: ('a => 'b, t('a)) => t(('a, 'b));
let flatAmend: ('a => t('b), t('a)) => t(('a, 'b));
let flatten: (t(t('a))) => t('a);
