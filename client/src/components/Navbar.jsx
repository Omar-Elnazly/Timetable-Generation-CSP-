import { useState } from "react";
import logo from "../assets/logo.ico";

const Navbar = () => {
    const [isOpen, setIsOpen] = useState(false);

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
                            behavior: "smooth",
                        });
                        setIsOpen(false);
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

                {/* Desktop Nav */}
                <nav className="hidden md:flex gap-6 text-md font-black text-white">
                    <a
                        href="#stack"
                        className="hover:text-gray-300 transition-colors duration-300"
                        onClick={(e) => {
                            e.preventDefault();
                            document.getElementById("stack")?.scrollIntoView({
                                behavior: "smooth",
                                block: "start",
                            });
                        }}
                    >
                        Tech Stack
                    </a>
                    <a
                        href="#performance"
                        className="hover:text-gray-300 transition-colors duration-300"
                        onClick={(e) => {
                            e.preventDefault();
                            document.getElementById("performance")?.scrollIntoView({
                                behavior: "smooth",
                                block: "start",
                            });
                        }}
                    >
                        Metrics
                    </a>
                    <a
                        href="#getstarted"
                        className="hover:text-gray-300 transition-colors duration-300"
                        onClick={(e) => {
                            e.preventDefault();
                            document
                                .getElementById("getstarted")
                                ?.scrollIntoView({
                                    behavior: "smooth",
                                    block: "start",
                                });
                        }}
                    >
                        Get Started
                    </a>
                </nav>

                 {/* Desktop Github Button */}
                <a href="https://github.com/Omar-Elnazly/Timetable-Generation-CSP-" className="hidden md:block">
                    <button className="rounded-xl px-4 py-2 text-sm font-medium duration-500 ease-in-out border-1 text-white hover:text-gray-300">
                        Github
                    </button>
                </a>

                {/* Mobile Menu Button */}
                <button
                    className="md:hidden text-white focus:outline-none"
                    onClick={() => setIsOpen(!isOpen)}
                >
                    {isOpen ? (
                        <svg className="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                            <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M6 18L18 6M6 6l12 12" />
                        </svg>
                    ) : (
                        <svg className="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                            <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M4 6h16M4 12h16M4 18h16" />
                        </svg>
                    )}
                </button>
            </div>

            {/* Mobile Menu Dropdown */}
            {isOpen && (
                <div className="md:hidden bg-black/95 backdrop-blur-xl border-t border-gray-800 absolute w-full left-0 top-full py-4 flex flex-col items-center gap-4 text-white font-bold transition-all duration-300 ease-in-out">
                    <a
                        href="#stack"
                        onClick={(e) => {
                            e.preventDefault();
                            setIsOpen(false);
                            document.getElementById("stack")?.scrollIntoView({ behavior: "smooth", block: "start" });
                        }}
                        className="hover:text-gray-300"
                    >
                        Tech Stack
                    </a>
                    <a
                        href="#performance"
                        onClick={(e) => {
                            e.preventDefault();
                            setIsOpen(false);
                            document.getElementById("performance")?.scrollIntoView({ behavior: "smooth", block: "start" });
                        }}
                        className="hover:text-gray-300"
                    >
                        Metrics
                    </a>
                    <a
                        href="#getstarted"
                        onClick={(e) => {
                            e.preventDefault();
                            setIsOpen(false);
                            document.getElementById("getstarted")?.scrollIntoView({ behavior: "smooth", block: "start" });
                        }}
                        className="hover:text-gray-300"
                    >
                        Get Started
                    </a>
                     <a href="https://github.com/Omar-Elnazly/Timetable-Generation-CSP-">
                        <button className="rounded-xl px-4 py-2 text-sm font-medium border border-gray-700 hover:border-white transition-colors">
                            Github
                        </button>
                    </a>
                </div>
            )}
        </header>
    );
};

export default Navbar;
