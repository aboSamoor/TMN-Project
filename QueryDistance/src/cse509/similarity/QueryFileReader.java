package cse509.similarity;

import java.io.*;
import java.util.*;
import de.linguatools.disco.*;

public class QueryFileReader implements Constants {
	private String DISCO_REPO;
	private String DISCO_OPT;
	private long scores[][];
	private Vector<String> queries;
	private Vector<String> queryTypes;
	
	QueryFileReader(String[] args) {
		BufferedReader bufferedReader;
		String line;
		int counter = 0;
		queries = new Vector<String>();
		queryTypes = new Vector<String>();
		
		System.out.println("Beginning to read the queries file.");
		DISCO_REPO = new String(args[0].toString());
		DISCO_OPT = new String(args[1].toString());
		
		/* Part 1: Read all input queries */
		try {
			bufferedReader = new BufferedReader(new FileReader(new File(QUERY_FILE_LOCATION)));
			
			while ((line = bufferedReader.readLine()) != null) {
				//Extract Column 1 : Query
				int col1Limit = line.indexOf(DELIMITER);
				String q = new String(line.substring(0, col1Limit));
				queries.add(q);
				
				//Extract Column 2 : Type
				int col2Limit = line.substring(col1Limit+1).indexOf(DELIMITER);
				String type = new String(line.substring((col1Limit+1), (col1Limit+col2Limit+1)));
				queryTypes.add(type);
				
				counter++;
			}
			
			bufferedReader.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		/* Part 2: Compute scores for each query */
		scores = new long[counter][counter];
		
		/* Initialize scores */
		for (int i = 0; i < counter; i++) {
			for (int j = 0; j < counter; j++) {
				scores[i][j] = -1;
			}
		}
		
		System.out.println("Done.");
	}
	
	private void iterateOverQueries() {
		long counter = queries.size();
		PrintStream p;
		System.out.println("Beginning semantic processing....");
		try {
			p = new PrintStream(new File(MATRIX_FILE_LOCATION));
			
			for (int i = 0; i < counter; i++) {
				p.format("%s ",queryTypes.get(i));
				for (int j = 0; j < counter; j++) {
					if (i == j)
						scores[i][j] = PRECISION;
					else if (scores[j][i] == -1)
						scores[i][j] = Math.round((PRECISION)*calculateScore(queries.elementAt(i), queries.elementAt(j)));
					else scores[i][j] = scores[j][i];
					
					p.format("%10d ", scores[i][j]);
				}
				p.println();
				System.out.println("Semantic processing of Query: " + i);
			}
			
			p.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}
	
	private double calculateScore(String s, String t) {
		double score;
		double maxScore = 0.0;
		double aggregate = 0.0;
		
		/* Step 1: Extract words from s and t */
		Vector<String> sWords = new Vector<String>();
		Vector<String> tWords = new Vector<String>();
		sWords = tokenize(s);
		tWords = tokenize(t);
				
		/* Step 2: Compute the score between each word */
		for (int i = 0; i < sWords.size(); i++) {
			for (int j = 0; j < tWords.size(); j++) {
				try {
					if (j%5 == 0)
						Runtime.getRuntime().gc();
					
					DISCO d = new DISCO();
					if (DISCO_OPT.equals("-s"))
						score = d.firstOrderSimilarity(DISCO_REPO, sWords.get(i), tWords.get(j));
					else
						score = d.secondOrderSimilarity(DISCO_REPO, sWords.get(i), tWords.get(j));
					
					if (score != -1) {
						if (maxScore < score)
							maxScore = score;
					} else
						continue;
				} catch (IOException e) {
					System.out.println("IOException. word1 = " + sWords.get(i) + " word2 = " + tWords.get(j));
					e.printStackTrace();
				}
			}
			aggregate+=maxScore;
		}
		
		//Return normalized score
		return (double)aggregate/(double)(Math.max(sWords.size(), tWords.size()));
	}
	
	private Vector<String> tokenize(String str) {
		Vector<String> v = new Vector<String>();
		StringTokenizer st = new StringTokenizer(str);
		
		while (st.hasMoreElements()) {
			v.add(st.nextToken());
		}
		
		return v;
	}
 	
	public static void main(String[] args) {

		if (args.length != 2) {
			System.out.println("Usage: java -jar Query.jar DISCO_REPO DISCO_OPT");
			System.exit(1);
		}
		
		QueryFileReader qfr = new QueryFileReader(args);
		qfr.iterateOverQueries();
	}
}
