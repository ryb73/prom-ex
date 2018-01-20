open Js.Promise;
open PromiseEx;

let (%>) = BatPervasives.(%>);

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
    |> tapMaybe(Js.log2("it's") %> resolve)
    |> tapMaybe((v) => {
        Js.log2("it's still", v);
        resolve();
    })
    |> thenResolve(None)
    |> tapMaybe(Js.log2("not this time") %> resolve);


[@bs.val] external setTimeout : (((unit) => unit), int) => unit = "";
let p = Js.Promise.make((~resolve, ~reject as _) => {
    setTimeout(u => [@bs] resolve(u), 10);
});

p
    |> then_(() => {
        Js.log("woah");
        Js.Exn.raiseError("done");
    })
    |> catch((exn) => {
        Js.log(exn);
        resolve ();
    });