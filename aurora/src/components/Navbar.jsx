import logo from "../assets/logo.ico";

const Navbar = () => {
    return (
        <header className="fixed top-0 left-0 w-full z-50 backdrop-blur-md bg-black/70 border-b border-gray-800/50">
            <div className="mx-auto flex max-w-screen-xl items-center justify-between px-6 py-4">
                <a 
                    href="#top" 
                    className="group flex items-center gap-3 cursor-pointer"
                    onClick={(e) => {
                        e.preventDefault();
                        window.scrollTo({
                            top: 0,
                            behavior: "smooth"
                        });
                    }}
                >
                    <img
                        src={logo}
                        alt="Logo"
                        className="w-8 h-8 object-contain transition duration-[3000ms] group-hover:brightness-125"
                    />
                    <span className="group-hover-gradient text-lg font-bold text-white transition duration-[5000ms]">
                        CSP Timetable
                    </span>
                </a>

                <nav className="flex gap-6 text-md font-black text-white">
                    <a
                        href="#philosophy"
                        className="hover:text-gray-300 transition-colors duration-300"
                        onClick={(e) => {
                            e.preventDefault();
                            document
                                .getElementById("philosophy")
                                ?.scrollIntoView({
                                    behavior: "smooth",
                                    block: "start",
                                });
                        }}
                    >
                        Technology
                    </a>
                    <a
                        href="#plans"
                        className="hover:text-gray-300 transition-colors duration-300"
                        onClick={(e) => {
                            e.preventDefault();
                            document.getElementById("plans")?.scrollIntoView({
                                behavior: "smooth",
                                block: "start",
                            });
                        }}
                    >
                        Performance
                    </a>
                    <a
                        href="#getstarted"
                        className="hover:text-gray-300 transition-colors duration-300"
                        onClick={(e) => {
                            e.preventDefault();
                            document.getElementById("getstarted")?.scrollIntoView({
                                behavior: "smooth",
                                block: "start",
                            });
                        }}
                    >
                        Get Started
                    </a>
                </nav>
                <a href="https://github.com/Omar-Elnazly/Timetable-Generation-CSP-">
                    <button className="rounded-xl px-4 py-2 text-sm font-medium duration-500 ease-in-out border-1">
                        Github
                    </button>
                </a>
            </div>
        </header>
    );
};

export default Navbar;
