open Js.Promise;

let map: ('a => 'b, t('a)) => t('b);
let thenResolve: ('a, t('b)) => t('a);
let tap: ('a => 'b, t('a)) => t('a);
let invertOptional: option(t('a)) => t(option('a));
let amend: ('a => 'b, t('a)) => t(('a, 'b));
let flatAmend: ('a => t('b), t('a)) => t(('a, 'b));
let flatten: (t(t('a))) => t('a);
let always: (unit => _, t('a)) => t('a);
