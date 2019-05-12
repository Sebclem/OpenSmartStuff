document.addEventListener('DOMContentLoaded', function () {
    let elems = document.querySelectorAll('.sidenav');
    M.Sidenav.init(elems);

    var drops = document.querySelector('#account-trigger');
    M.Dropdown.init(drops, {
        coverTrigger: false,
        alignment: 'right',
        onOpenStart: () => {
            let drop = document.querySelector('#li-drop-account');
            if (!drop.classList.contains("active"))
                drop.classList.add("active");
        },
        onCloseStart: () => {
            let drop = document.querySelector('#li-drop-account');
            if (drop.classList.contains("active"))
                drop.classList.remove("active");
        }
    });

    document.querySelector('#force-google-sync').onclick = function () {
        $.post("/api/forceGoogleSync", function () {
            M.toast({
                html: '<i class="material-icons" style="margin-right: 10px">check_circle</i> Request send !<button class="btn-flat toast-action white-text" onclick="M.Toast.getInstance(this.parentElement).dismiss();"><i class="material-icons">delete_sweep</i></button>',
                classes: 'rounded green',
                displayLength: 10000
            });
        }).fail(function (state) {
            if(state.status === 412){
                M.toast({
                    html: '<i class="material-icons" style="margin-right: 10px">info_outline</i> You need to link Google Home before<button class="btn-flat toast-action black-text" onclick="M.Toast.getInstance(this.parentElement).dismiss();"><i class="material-icons">delete_sweep</i></button>',
                    classes: 'rounded orange  black-text',
                    displayLength: 10000
                });
            }else if(state.status === 401){
                M.toast({
                    html: '<i class="material-icons" style="margin-right: 10px">warning</i>Your are not logged in !<button class="btn-flat toast-action white-text" onclick="M.Toast.getInstance(this.parentElement).dismiss();"><i class="material-icons">delete_sweep</i></button>',
                    classes: 'rounded red',
                    displayLength: 10000
                });
            }
            else{
                M.toast({
                    html: '<i class="material-icons" style="margin-right: 10px">warning</i>Internal Server Error<button class="btn-flat toast-action white-text" onclick="M.Toast.getInstance(this.parentElement).dismiss();"><i class="material-icons">delete_sweep</i></button>',
                    classes: 'rounded red',
                    displayLength: 10000
                });
            }
        })
    }
});