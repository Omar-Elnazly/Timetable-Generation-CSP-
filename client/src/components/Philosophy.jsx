import { motion } from "framer-motion";

const Philosophy = () => {
    return (
        <section
            id="philosophy"
            className="min-h-screen bg-black flex flex-col items-center justify-center px-4 text-center"
        >
            <motion.div
                initial={{ opacity: 0, y: 50 }}
                whileInView={{ opacity: 1, y: 0 }}
                transition={{ duration: 0.8, ease: "easeOut" }}
                viewport={{ once: true, amount: 0.3 }}
                className="max-w-4xl mx-auto"
            >
                <h2 className="text-5xl sm:text-6xl lg:text-7xl font-bold text-white mb-4">
                    <span className="gradient-text">Technology</span>
                </h2>
                <div className="space-y-6">
                    <p className="text-xl sm:text-2xl text-gray-300 leading-relaxed font-light">
                        "Efficient algorithms solve complex problems elegantly."
                    </p>
                    <p className="text-lg text-gray-400 leading-relaxed max-w-3xl mx-auto">
                        Built with C++ for maximum performance, leveraging SQLite for data persistence,
                        and powered by sophisticated CSP algorithms. The system generates optimal timetables
                        for INSAT Computer Science department across all academic years and specializations.
                    </p>
                </div>
            </motion.div>
        </section>
    );
};

export default Philosophy;
