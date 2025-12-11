import { motion } from "framer-motion";
import { useEffect } from "react";
import "./App.css";
import Navbar from "./components/Navbar";
import Hero from "./components/Hero";
import Philosophy from "./components/Philosophy";
import Plans from "./components/Plans";
import GetStarted from "./components/GetStarted";

const App = () => {
    // Scroll to top on initial load
    useEffect(() => {
        window.scrollTo({
            top: 0,
            behavior: "smooth",
        });
    }, []);

    return (
        <div className="scroll-smooth">
            {/* Navigation */}
            <motion.nav
                initial={{ opacity: 0 }}
                animate={{ opacity: 1 }}
                transition={{ duration: 1, delay: 1, ease: "easeIn" }}
            >
                <Navbar />
            </motion.nav>

            {/* Sections */}
            <Hero />
            <Philosophy />
            <Plans />
            <GetStarted />
        </div>
    );
};

export default App;
