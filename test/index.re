open Js.Promise;
open PromiseEx;

resolve(2)
    |> map((x) => x * 2)
    |> tap((_) => {
        Js.log("ok");
        resolve()
    })
    |> tap((ok) => Js.log(ok))
    |> thenResolve("lol")
    |> map(Js.log)
    |> thenResolve(Some(9))
    |> tapMaybe(Js.log2("it's"))
    |> tapMaybe(Js.log2("it's still"))
    |> thenResolve(None)
    |> tapMaybe(Js.log2("not this time"));
