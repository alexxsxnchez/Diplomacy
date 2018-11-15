

enum DecisionResult {
	YES,
	NO,
	UNDECIDED
};



class Decision {

	private:
		std::vector<Decision *> dependencies_;
		Move * move_;
	
	public:
		
		boolean update(Move * )
		boolean updateDependencies();


};