import { motion } from "framer-motion";
import githubLogo from "../assets/github.png";

const Hero = () => {
    return (
        <section className="min-h-screen flex flex-col items-center justify-center px-4 text-center">
            <motion.h1
                initial={{ opacity: 0, y: -50, scale: 1.05 }}
                animate={{ opacity: 1, y: 0, scale: 1 }}
                transition={{ duration: 2, ease: "easeInOut" }}
                className="gradient-text font-bold text-7xl sm:text-8xl lg:text-8xl mb-6"
            >
                CSP Timetable
            </motion.h1>

            <motion.a
                href="https://github.com/Omar-Elnazly/Timetable-Generation-CSP-"
                target="_blank"
                rel="noopener noreferrer"
                initial={{ opacity: 0, y: -20 }}
                animate={{ opacity: 1, y: 0 }}
                transition={{ duration: 1, delay: 1 }}
                className="text-base sm:text-lg mb-10 inline-flex items-center gap-2"
            >
                <code
                    className="
      text-xs md:text-base lg:text-lg
      text-gray-400
      transition-colors duration-500 ease-in-out
      hover:text-white
    "
                >
                    &lt; Intelligent scheduling powered by Constraint Satisfaction Problem algorithms{" "}
                    <img
                        src={githubLogo}
                        alt="GitHub logo"
                        loading="lazy"
                        className="w-5 h-5 inline-block"
                    />{" "}
                    /&gt;
                </code>
            </motion.a>

            <motion.a
                href="#getstarted"
                initial={{ opacity: 0, scale: 0.9 }}
                animate={{ opacity: 1, scale: 1 }}
                transition={{ duration: 1, delay: 1, ease: "easeInOut" }}
                className="group relative inline-block focus:outline-none max-w-full"
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
                <span className="absolute inset-0 rounded-xl border-2 border-green-900 transition group-hover:scale-104 group-hover:rounded-2xl" />
                <span className="relative z-10 flex items-center justify-center rounded-xl bg-white px-6 sm:px-10 lg:px-12 py-3 text-sm sm:text-base font-medium text-green-600 transition-all duration-500 group-hover:-translate-x-1 group-hover:-translate-y-1 group-hover:bg-black group-hover:text-white truncate">
                    View Demo
                </span>
            </motion.a>
        </section>
    );
};

export default Hero;
