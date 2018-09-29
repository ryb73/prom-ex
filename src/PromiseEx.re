open Reduice.Promise;
open Belt.Result;

let map = (callback) => then_((v) => resolve(callback(v)));

let tap = (callback) => map((v) => {
    callback(v);
    v;
});

let thenResolve = (v) => then_((_) => resolve(v));

let invertOptional = fun
    | None => resolve(None)
    | Some(p) => map((v) => Some(v), p);

let thenMaybe = (callback) => then_ (fun
    | None => resolve(None)
    | Some(v) => callback(v)
        |> map((v) => Some(v))
);

let mapMaybe = (callback) => map(fun
    | None => None
    | Some(v) => Some(callback(v))
);

let tapMaybe = (callback) => tap(fun
    | None => resolve()
    | Some(v) => callback(v)
);

exception EmptyOption;
let _getOpt = fun
    | Some(v) => v
    | _ => raise(EmptyOption);

let unwrapResult = (promise) => map(
    fun
    | Ok(v) => v
    | Error(e) => e |> Obj.magic |> raise,
    promise
);
