open Js.Promise;

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

let amend = (f) => map(v => (v, f(v)));

let flatAmend = (f) => then_(v => all2((resolve(v), f(v))));

let flatten = (outer) => then_(inner => inner, outer);

let always = (cb, promise) =>
    promise
    |> catch(exn => {
        resolve(cb())
        |> catch(_ => reject(Obj.magic(exn)))
        |> then_(_ => reject(Obj.magic(exn)))
    })
    |> tap(_ => cb());

let delay = (ms) =>
    Js.Promise.make((~resolve, ~reject as _) =>
        Js.Global.setTimeout(u => resolve(. u), ms)
        |> ignore
    );

module Then = {
    let let_ = (p, cb) => then_(cb, p);
    let try_ = (p, cb) => catch(cb, p);
};
