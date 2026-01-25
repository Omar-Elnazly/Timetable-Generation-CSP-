import { motion } from "framer-motion";

const Stack = () => {
    return (
        <section
            id="stack"
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
                    <span className="gradient-text">Tech Stack</span>
                </h2>
                <div className="space-y-6">
                    <p className="text-xl sm:text-2xl text-gray-300 leading-relaxed font-light">
                        Built for speed and reliability.
                    </p>
                    <div className="flex flex-wrap justify-center gap-4 mt-8">
                        <div className="px-6 py-3 rounded-full bg-blue-900/30 border border-blue-500/50 text-blue-300 font-bold text-xl">
                            C++ 17
                        </div>
                        <div className="px-6 py-3 rounded-full bg-cyan-900/30 border border-cyan-500/50 text-cyan-300 font-bold text-xl">
                            React 19
                        </div>
                        <div className="px-6 py-3 rounded-full bg-purple-900/30 border border-purple-500/50 text-purple-300 font-bold text-xl">
                            SQLite
                        </div>
                        <div className="px-6 py-3 rounded-full bg-yellow-900/30 border border-yellow-500/50 text-yellow-300 font-bold text-xl">
                            Vite
                        </div>
                    </div>
                </div>
            </motion.div>
        </section>
    );
};

export default Stack;
