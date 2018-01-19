open Js.Promise;
open Js.Result;

let map callback => then_ @@ fun v => resolve @@ callback v;

let tap callback => map @@ fun v => {
    callback v;
    v;
};

let thenResolve v => then_ @@ fun _ => resolve v;

let invertOptional = fun
    | None => resolve None
    | Some p => map (fun v => Some v) p;

let thenMaybe callback => then_ (fun
    | None => resolve None
    | Some v => callback v |> map (fun v => Some v)
);

let mapMaybe callback => map (fun
    | None => None
    | Some v => Some (callback v)
);

let tapMaybe callback => tap(fun
    | None => ()
    | Some v => callback v
);

exception EmptyOption;
let _getOpt = fun
    | Some v => v
    | _ => raise EmptyOption;

let unwrapResult promise => map (fun
    | Ok v => v
    | Error e => e |> Obj.magic |> raise
) promise;