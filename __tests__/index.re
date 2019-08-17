open Jest;
open Expect;
open Js.Promise;
open PromEx;

[@bs.val] external setTimeout : (((unit) => unit), int) => unit = "";
let p = Js.Promise.make((~resolve, ~reject as _) => {
    setTimeout(u => [@bs] resolve(u), 10);
});

testPromise("map", () =>
    resolve(2)
    |> map((x) => x * 2)
    |> map(expect)
    |> map(toBe(4))
);

testPromise("thenResolve", () =>
    resolve(2)
    |> thenResolve(3)
    |> map(expect)
    |> map(toBe(3))
);

describe("tap", () => {
    testPromise("sync", () => {
        let tapped = ref(0);
        resolve(2)
        |> tap(_ => incr(tapped))
        |> map(_ => tapped^)
        |> map(expect)
        |> map(toBe(1));
    });

    testPromise("async", () => {
        let tapped = ref(0);
        resolve(2)
        |> tap(_ =>
            Js.Promise.make((~resolve, ~reject as _) =>
                Js.Global.setTimeout(() => {
                    incr(tapped);
                    resolve(. ():unit);
                }, 500)
                |> ignore
            )
        )
        |> map(_ => tapped^)
        |> map(expect)
        |> map(toBe(1));
    });

    testPromise("option", () => {
        let num = ref(0);
        resolve(Some(()))
        // make sure this
        |> tap(Belt.Option.map(_, () => {
            delay(500)
            |> map(_ => num := (num^ + 1) * 2);
        }))
        // runs before this
        |> tap(_ => num := (num^ + 2) * 2)
        |> map(_ => num^)
        |> map(expect)
        |> map(toBe(8))
    });
});

describe("invertOptional", () => {
    testPromise("some", () =>
        Some(resolve(1))
        |> invertOptional
        |> map(expect)
        |> map(toBe(Some(1)))
    );

    testPromise("none", () =>
        None
        |> invertOptional
        |> map(expect)
        |> map(toBe(None))
    );
});

describe("thenMaybe", () => {
    testPromise("some", () =>
        resolve(Some(1))
        |> thenMaybe(v => resolve(v + 1))
        |> map(expect)
        |> map(toBe(Some(2)))
    );

    testPromise("none", () =>
        resolve(None)
        |> thenMaybe(_ => failwith("no"))
        |> map(expect)
        |> map(toBe(None))
    );
});

testPromise("amend", () =>
    resolve(2)
    |> amend(x => x * 2)
    |> map(expect)
    |> map(toEqual((2, 4)))
);

testPromise("flatAmend", () =>
    resolve(2)
    |> flatAmend(x => resolve(x * 2))
    |> map(expect)
    |> map(toEqual((2, 4)))
);

testPromise("flatten", () =>
    resolve(resolve(100))
    |> flatten
    |> map(expect)
    |> map(toBe(100))
);

describe("always", () => {
    describe("sync", () => {
        testPromise("resolve", () => {
            let tapped = ref(0);
            resolve(2)
            |> always(() => incr(tapped))
            |> map(_ => tapped^)
            |> map(expect)
            |> map(toBe(1))
        });

        testPromise("reject", () => {
            let tapped = ref(0);
            reject(Failure("oof"))
            |> always(() => incr(tapped))
            |> catch(_ => resolve(-100))
            |> map(_ => tapped^)
            |> map(expect)
            |> map(toBe(1))
        });
    });

    describe("async", () => {
        testPromise("resolve", () => {
            let tapped = ref(0);
            resolve(2)
            |> always(() =>
                Js.Promise.make((~resolve, ~reject as _) =>
                    Js.Global.setTimeout(() => {
                        incr(tapped);
                        resolve(. ():unit);
                    }, 500)
                    |> ignore
                )
            )
            |> map(_ => tapped^)
            |> map(expect)
            |> map(toBe(1))
        });

        describe("reject", () => {
            testPromise("delay", () => {
                let tapped = ref(0);
                reject(Failure("oof"))
                |> always(() =>
                    Js.Promise.make((~resolve, ~reject as _) =>
                        Js.Global.setTimeout(() => {
                            incr(tapped);
                            resolve(. ():unit);
                        }, 500)
                        |> ignore
                    )
                )
                |> catch(_ => resolve(-100))
                |> map(_ => tapped^)
                |> map(expect)
                |> map(toBe(1));
            });

            testPromise("always succeeds", () => {
                let exn = Failure("woah");
                reject(exn)
                |> always(() =>
                    Js.Promise.make((~resolve, ~reject as _) =>
                        Js.Global.setTimeout(() => {
                            resolve(. ():unit);
                        }, 500)
                        |> ignore
                    )
                )
                |> catch(resolve)
                |> map(expect)
                |> map(toBe(Obj.magic(exn)));
            });

            testPromise("always fails", () => {
                let exn = Failure("woah");
                reject(exn)
                |> always(() =>
                    Js.Promise.make((~resolve as _, ~reject) =>
                        Js.Global.setTimeout(() => {
                            reject(. Failure("oof"));
                        }, 500)
                        |> ignore
                    )
                )
                |> catch(resolve)
                |> map(expect)
                |> map(toBe(Obj.magic(exn)));
            });
        });
    });
});
