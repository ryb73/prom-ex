open Reduice.Promise;
open Belt.Result;

let map = (callback) => then_(v => resolve(callback(v)));

let thenResolve = (v) => then_(_ => resolve(v));

let tap = (callback) => then_(v => {
    /* if callback returns a promise, then we want to wait for that promise
       to resolve. we don't know if it does return a promise, but if we wrap
       it in `resolve` then it doesn't matter */
    resolve(callback(v))
    |> thenResolve(v);
});

let invertOptional = fun
    | None => resolve(None)
    | Some(p) => map(v => Some(v), p);

let thenMaybe = (callback) => then_(fun
    | None => resolve(None)
    | Some(v) =>
        callback(v)
        |> map(result => Some(result))
);

let mapMaybe = (callback) => map(fun
    | None => None
    | Some(v) => Some(callback(v))
);

let tapMaybe = (callback, promise) =>
    promise
    |> tap(fun
        | None => ()
        | Some(v) => callback(v) |> ignore
    );

let unwrapResult = (promise) => map(
    fun
    | Ok(v) => v
    | Error(e) => e |> Obj.magic |> raise,
    promise
);

let amend = (f) => map(v => (v, f(v)));

let flatAmend = (f) => then_(v => all2((resolve(v), f(v))));
